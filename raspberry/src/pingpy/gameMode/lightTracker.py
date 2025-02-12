from abc import ABC, abstractmethod
from ..input.input import Input
from ..output.output import Output
from .gameMode import GameMode
from pingpy.config.config import BLUE, YELLOW, PATH_AUDIO_LIGHT_TRACKER_INTRO, WHITE, PATH_AUDIO_GAGNE, PATH_AUDIO_PLAYER_BLEU, PATH_AUDIO_PLAYER_ROUGE, PATH_AUDIO_PLAYER_JAUNE, PATH_AUDIO_PLAYER_VERT, PATH_AUDIO_LIGHT_TRACKER_BEGIN_ROUND, PATH_AUDIO_LIGHT_TRACKER_RED_PLAYER_WIN_ROUND, PATH_AUDIO_LIGHT_TRACKER_BLUE_PLAYER_WIN_ROUND, PATH_AUDIO_LIGHT_TRACKER_YELLOW_PLAYER_WIN_ROUND, PATH_AUDIO_LIGHT_TRACKER_GREEN_PLAYER_WIN_ROUND, RED
import time
from pingpy.debug import logger
from random import uniform    
    
class LightTracker(GameMode):
    def __init__(self):
        self.color = BLUE
        self.descriptionAudioPath = PATH_AUDIO_LIGHT_TRACKER_INTRO
        self.currentState = "setup"
        self.playerRemaningMoves = [None for _ in range(4)] # None is non_playing, if integer, it is the number of moves left, 0 is no more moves
        self.beginRoundTime = 0
        self.roundTimeOut = 8
        self.endRoundTempo = 2 # Time before the end of the round, to show the result
        self.evaluateTime = 0
        self.winningRound = 3
        self.playerScores = [None for _ in range(4)]   
        self.playerError = [None for _ in range(4)]
        self.target = None    
        self.minNewTargetDistance = 50 
        self.targetRange = [-120, 120]
        self.lightWith = 10
        self.minLightWith = 5
        self.maxLightWith = 40
        self.minPlayingSpeed = 30
        self.maxPlayingSpeed = 500
        self.minPlayingAcceleration = self.minPlayingSpeed * 10
        self.maxPlayingAcceleration = self.maxPlayingSpeed * 0.1
        self.playingSpeed = self.minPlayingSpeed
        self.playingAcceleration = self.minPlayingAcceleration

        
    def updateDifficulty(self, Input):
        if Input.UICorner.level is not None:
            self.playingSpeed = self.minPlayingSpeed + (Input.UICorner.level * (self.maxPlayingSpeed - self.minPlayingSpeed))
            self.playingAcceleration = self.minPlayingAcceleration + (Input.UICorner.level * (self.maxPlayingAcceleration - self.minPlayingAcceleration))
            self.lightWith = self.minLightWith + (Input.UICorner.level * (self.maxLightWith - self.minLightWith))
            Input.UICorner.level = None
            

        
        
    def setup(self, Input, Output):
        self.playerScores = [0 for _ in range(4)]  
        self.currentState = "setupIDLE"
        for i in range(4):
            Output.player[i].linearActuator.moveTo = 0
            Output.player[i].playerLedStrip.area = [-200, 200] 
            Output.player[i].playerLedStrip.color = (0, 0, 0)
            if Input.player[i].gameController.left == False or Input.player[i].gameController.right == False:
                Input.player[i].gameController.left = None
                Input.player[i].gameController.right = None
                if Input.player[i].linearActuator.moving:
                    Output.player[i].linearActuator.stop = True
                    
            if Input.player[i].usb.connected:
                self.playerScores[i] = 0
                print(f"Player {i} is playing light tracker")
                self.playerError[i] = None
            else:
                self.playerScores[i] = None





    
    def compute(self, input, output):
        self.updateDifficulty(input)
        if self.currentState == "setup":
            self.setup(input, output)
            self.currentState = "setupIDLE"
            return
        if self.currentState == "setupIDLE":
            if output.speaker.isBusy == True:
                return
            for i in range(4):
                if input.player[i].linearActuator.moving == True:
                    return
            self.currentState = "newRound"
            return
        if self.currentState == "newRound":
            self.newRound(input, output)
            self.currentState = "newRoundIDLE"
            return
        if self.currentState == "newRoundIDLE":
            self.handlePlayerMove(input, output)
            if all([i is None or i == 0 for i in self.playerRemaningMoves]):
                logger.write_in_log("INFO", __name__, "compute", "All players have played")
                self.currentState = "evaluate"
                return
            if  time.time() - self.beginRoundTime > self.roundTimeOut:
                logger.write_in_log("INFO", __name__, "compute", "Timed out")
                self.currentState = "evaluate"
                return
        if self.currentState == "evaluate":
            self.evaluate(input, output)
            self.currentState = "evaluateIDLE"
            return
        if self.currentState == "evaluateIDLE":
            for i in range(4):
                if input.player[i].linearActuator.moving == True:
                    return
            if time.time() - self.evaluateTime > self.endRoundTempo:
                self.currentState = "endRound"
            return
        if self.currentState == "endRound":
            self.endRound(input, output)
            self.currentState = "endRoundIDLE"
            return
        if self.currentState == "endRoundIDLE":
            for i in range(4):
                if input.player[i].linearActuator.moving == True:
                    return
            if output.speaker.isBusy == False: # wait for the end of the sound
                for i in self.playerScores:
                    if i is None:
                        continue
                    if i >= self.winningRound:
                        self.currentState = "endGame"
                        return
                self.currentState = "newRound"
            return

        if self.currentState == "endGame":
            self.endGame(input, output)
            self.currentState = "endGameIDLE"

    
    def newRound(self, Input, Output):
        logger.write_in_log("INFO", __name__, "newRound", "New round")
        self.beginRoundTime = time.time()
        for i in range(4):
            if self.playerScores[i] is not None:
                self.playerRemaningMoves[i] = 1
                Input.player[i].gameController.left = None
                Input.player[i].gameController.right = None
                
        if self.target is None:
            self.target = uniform(self.targetRange[0]+self.lightWith, self.targetRange[1]-self.lightWith)
        Output.speaker.audioPiste = PATH_AUDIO_LIGHT_TRACKER_BEGIN_ROUND

        lastTarget = self.target 
        while abs(self.target - lastTarget) < self.minNewTargetDistance:
            self.target = uniform(self.targetRange[0], self.targetRange[1])
        
        for i in range(4):
            Output.player[i].playerLedStrip.area = [self.target - self.lightWith/2, self.target + self.lightWith/2] 
            Output.player[i].playerLedStrip.color = self.color 
            if not Input.player[i].usb.connected:
               Output.player[i].playerLedStrip.color = (0, 0, 0)
            else :
                Output.player[i].playerLedStrip.color = RED

        
        # flush the game controller
        Input.player[i].gameController.left = None
        Input.player[i].gameController.right = None
    
        
    
    def handlePlayerMove(self, Input, Output):
        for i in range(4):
            if self.playerRemaningMoves[i] is None or not Input.player[i].usb.connected:
                continue
            if self.playerRemaningMoves[i] <= 0 :
                continue
            
            if Input.player[i].gameController.left == True:
                Output.player[i].linearActuator.setMaxSpeed = self.playingSpeed
                Output.player[i].linearActuator.setMaxAccel = self.playingAcceleration
                Input.player[i].gameController.left = None
                Output.player[i].linearActuator.moveToLeft = True
            elif Input.player[i].gameController.right == True:
                Output.player[i].linearActuator.setMaxSpeed = self.playingSpeed
                Output.player[i].linearActuator.setMaxAccel = self.playingAcceleration
                Input.player[i].gameController.right = None
                Output.player[i].linearActuator.moveToRight = True
            elif Input.player[i].gameController.left == False or Input.player[i].gameController.right == False:
                logger.write_in_log("INFO", __name__, "handlePlayerMove", "Stop the player")
                Input.player[i].gameController.left = None
                Input.player[i].gameController.right = None
                Output.player[i].linearActuator.stop = True
                self.playerRemaningMoves[i] -= 1
    
    def evaluate(self, Input, Output):
        logger.write_in_log("INFO", __name__, "evaluate", "Evaluate the round")
        self.evaluateTime = time.time()
        for i in range(4):
            if self.playerScores[i] is not None:
                self.playerError[i] = abs(Input.player[i].linearActuator.currentPose - self.target)
                print(f"Player {i} error: {self.playerError[i]}")
            if Input.player[i].linearActuator.moving:
                Output.player[i].linearActuator.stop = True

        
    
    def endRound(self, Input, Output):
        logger.write_in_log("INFO", __name__, "endRound", "End of the round")
        for i in range(4):
            if not Input.player[i].usb.connected:
               continue
            Output.player[i].linearActuator.setMaxSpeed = 300
            Output.player[i].linearActuator.setMaxAccel = 500
            Output.player[i].linearActuator.moveTo = self.target
            
        # i = self.playerError.index(min(self.playerError)) # ok but ignore not connected players
        i = -1
        minError = float('inf')
        for j in range(4):
            if self.playerError[j] is None :
                continue
            if self.playerError[j] < minError:
                minError = self.playerError[j]
                i = j
        if i == -1:
            logger.write_in_log("INFO", __name__, "endRound", "No player connected")
        if self.playerScores[i] is not None:
            self.playerScores[i] += 1
        # should play audio to announce the winner of the round

        
        if i == 0:
            Output.speaker.audioPiste = PATH_AUDIO_LIGHT_TRACKER_YELLOW_PLAYER_WIN_ROUND
        elif i == 1:
            Output.speaker.audioPiste = PATH_AUDIO_LIGHT_TRACKER_GREEN_PLAYER_WIN_ROUND
        elif i == 2:
            Output.speaker.audioPiste = PATH_AUDIO_LIGHT_TRACKER_RED_PLAYER_WIN_ROUND
        elif i == 3:
            Output.speaker.audioPiste = PATH_AUDIO_LIGHT_TRACKER_BLUE_PLAYER_WIN_ROUND

            
    def endGame(self, Input, Output):
        # i = self.playerScores.index(max(self.playerScores))
        i = -1
        maxScore = -1
        for j in range(4):
            if self.playerScores[j] is None:
                continue
            if self.playerScores[j] > maxScore:
                maxScore = self.playerScores[j]
                i = j
        
        if i == 0:
            Output.speaker.audioPiste = PATH_AUDIO_PLAYER_JAUNE
        elif i == 1:
            Output.speaker.audioPiste = PATH_AUDIO_PLAYER_VERT
        elif i == 2:
            Output.speaker.audioPiste = PATH_AUDIO_PLAYER_ROUGE
        elif i == 3:
            Output.speaker.audioPiste = PATH_AUDIO_PLAYER_BLEU
        Output.speaker.audioPiste = [Output.speaker.audioPiste]
        Output.speaker.audioPiste.append(PATH_AUDIO_GAGNE)
    
    def stop(self, output_ptr):
        """
        Stops the game and resets the outputs.
        """
        self.inGame = False
        
        for i in range(4):
            output_ptr.player[i].linearActuator.stop = True
            
        logger.write_in_log("INFO", __name__, "stop", "Game stopped.")