from abc import ABC, abstractmethod
from ..input.input import Input
from ..output.output import Output
from .gameMode import GameMode
from pingpy.config.config import BLUE, YELLOW, PATH_AUDIO_LIGHT_TRACKER_INTRO, WHITE, PATH_AUDIO_GAGNE, PATH_AUDIO_PLAYER_BLEU, PATH_AUDIO_PLAYER_ROUGE, PATH_AUDIO_PLAYER_JAUNE, PATH_AUDIO_PLAYER_VERT
import time
from pingpy.debug import logger
from random import uniform



# class LightTracker(GameMode):
#     """
#     Game mode of Light Tracker. Traque-lumière in french.
#     """
#     def __init__(self):
#         self.areaLed=0
#         self.ledCenter=0
#         self.actualRound=0
#         self.areaLength=20 ### ceci va varier selon la dificulté du jeu
#         self.color = WHITE
#         self.descriptionAudioPath = PATH_AUDIO_LIGHT_TRACKER_INTRO

#         logger.write_in_log("INFO", __name__, "__init__", "Game mode initialized.")

#     def setup(self, Input, Output):
#         """
#         Setup the game mode.
#         """
        
#         self.ledCenter = uniform(-200+self.areaLength/2 ,200 - self.areaLength/2)
#         self.areaLed = [self.ledCenter - self.areaLength/2, self.ledCenter + self.areaLength/2]
#         for i in range(4):
#             try:
#                 playerOutput = Output.player[i]
#                 playerInput = Input.player[i]
#                 playerOutput.linearActuator.moveTo = 0
#                 playerOutput.playerLedStrip.area = self.areaLed
#             except IndexError:
#                 logger.write_in_log("ERROR", __name__, "setup", f"No output found for player ID {Input.playerInput[i]}.")

#         self.actualRound += 1
#         pass
    
#     def compute(self, Input, Output):
#         """
#         Run the game mode.
#         """
#         if self.actualRound == 0:
#             self.setup(Input, Output)
#         for i in range(4):
#             if self.check_victory(Input.player[i], Output):
#                 self.stop(Input, Output)
#                 return
#             if not self.check_end_move(Input):
#                 self.move(Input.player[i], Output.player[i])    
#             else:
#                 winnerId = self.check_distance(Input)
#                 for winner in winnerId:
#                     self.modif_score(Input.player[winner])
#         self.new_round(Input, Output)

#     def stop(self, Input, Output):
#         logger.write_in_log("INFO", __name__, "stop", "Game stopped.")
        
#         for i in range(4):
#             Output.player[i].linearActuator.stop = True

#     def check_distance(self,Input):
#         winnerId = []
#         distance = float('inf')
#         for i in range(4):
#             new_distance = abs(Input.player[i].linearActuator.currentPose - self.ledCenter)
#             if new_distance <= distance:
#                 distance = new_distance
#                 winnerId.append(i)
#         return winnerId 
        

#     def check_move(self, PlayerInput):
#         return (
#             PlayerInput.gameController.countMove == 0 and
#             (PlayerInput.gameController.left or PlayerInput.gameController.right)
#         )

#     def move(self, PlayerInput, PlayerOutput):
#         if self.check_move(PlayerInput):
#             PlayerInput.gameController.countMove -= 1
#             if PlayerInput.gameController.left:
#                 PlayerOutput.linearActuator.moveToLeft = True
#                 PlayerOutput.linearActuator.moveToRight = False
#             elif PlayerInput.gameController.right:
#                 PlayerOutput.linearActuator.moveToLeft = False
#                 PlayerOutput.linearActuator.moveToRight = True
#         else:
#             PlayerOutput.linearActuator.moveToLeft = False
#             PlayerOutput.linearActuator.moveToRight = False
        
#     def check_end_move(self, Input):
#         return all(not self.check_move(Input.player[i]) for i in range(4))

#     def new_round(self,Input,Output):
#         for i in range(4):
#             Output.player[i].linearActuator.moveTo = self.ledCenter
#         newLedCenter = uniform(-200.0,200.0)
#         # newLedCenter = uniform(Input.player.playerLedStrip.rightLimit, Input.PlayerInput.playerLedStrip.leftLimit)

#         newPositionLed = newLedCenter
#         if newLedCenter == self.ledCenter:
#             newPositionLed = uniform(-200.0,200.0)
#             # newPositionLed = uniform(Input.player.playerLedStrip.rightLimit, Input.PlayerInput.playerLedStrip.leftLimit)
#         self.ledCenter = newPositionLed
#         self.areaLed = [self.ledCenter - self.areaLength/2, self.ledCenter + self.areaLength/2]
#         self.actualRound += 1 
#         for i in range(4):
#             Output.player[i].playerLedStrip.area = self.areaLed
#             Output.player[i].linearActuator.stop = False
#             Output.player[i].linearActuator.moveToLeft = False
#             Output.player[i].linearActuator.moveToRight = False
#         # Output.PlayerOutput.LinearActuatorOutput.move_to = self.ledCenter # changer avec linearActuator ?
    
#         pass

#     def modif_score(self,PlayerInput):
#         PlayerInput.pointCounter += 1
#         pass


#     def check_victory(self,Input, Output):
#         # for i in range(4):
#             # if Input.player[i].pointCounter == 3:
#             #     self.winnerID = i
#             #     return True
#         return False
    
    
class LightTracker(GameMode):
    def __init__(self):
        self.color = BLUE
        self.descriptionAudioPath = PATH_AUDIO_LIGHT_TRACKER_INTRO
        self.currentState = "setup"
        self.playerRemaningMoves = [None for _ in range(4)] # None is non_playing, if integer, it is the number of moves left, 0 is no more moves
        self.beginRoundTime = 0
        self.roundTimeOut = 5
        self.endRoundTempo = 2 # Time before the end of the round, to show the result
        self.evaluateTime = 0
        self.winningRound = 3
        self.playerScores = [0 for _ in range(4)]   
        self.playerError = [None for _ in range(4)]
        self.target = None    
        self.minNewTargetDistance = 50 
        self.targetRange = [-100, 100]
        self.lightWith = 20
        self.playingSpeed = 100
        self.playingAcceleration = 300
        
        
    def setup(self, Input, Output):
        self.playerScores = [0 for _ in range(4)]  
        self.currentState = "setupIDLE"
        for i in range(4):
            Output.player[i].linearActuator.moveTo = 0
            Output.player[i].playerLedStrip.area = [-200, 200] 
            Output.player[i].playerLedStrip.color = (0, 0, 0)




    
    def compute(self, input, output):
        if self.currentState == "setup":
            self.setup(input, output)
            self.currentState = "setupIDLE"
            return
        if self.currentState == "setupIDLE":
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
            if all([i == 0 for i in self.playerRemaningMoves]) or time.time() - self.beginRoundTime > self.roundTimeOut:
                self.currentState = "evaluate"
            return
        if self.currentState == "evaluate":
            self.evaluate(input, output)
            self.currentState = "evaluateIDLE"
            return
        if self.currentState == "evaluateIDLE":
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
                if all([i >= self.winningRound for i in self.playerScores]):
                    self.currentState = "endGame"
                else:
                    self.currentState = "newRound"
            return

        if self.currentState == "endGame":
            self.endGame(input, output)
            self.currentState = "endGameIDLE"

    
    def newRound(self, Input, Output):
        self.beginRoundTime = time.time()
        for i in range(4):
            if Input.player[i].usb.connectedFlag:
                self.playerRemaningMoves[i] = 1
                
        if self.target is None:
            self.target = uniform(self.targetRange[0], self.targetRange[1])
        
        lastTarget = self.target 
        while abs(self.target - lastTarget) < self.minNewTargetDistance:
            self.target = uniform(self.targetRange[0], self.targetRange[1])
        
        for i in range(4):
            Output.player[i].playerLedStrip.area = [self.target - self.lightWith/2, self.target + self.lightWith/2] 
            Output.player[i].playerLedStrip.color = self.color     
        
    
    def handlePlayerMove(self, Input, Output):
        for i in range(4):
            if self.playerRemaningMoves[i] == 0 or self.playerRemaningMoves[i] is None:
                continue
            if Input.player[i].gameController.left == True:
                Output.player[i].linearActuator.moveToLeft = True
            elif Input.player[i].gameController.right == True:
                Output.player[i].linearActuator.moveToRight = True
            elif Input.player[i].gameController.left == False or Input.player[i].gameController.right == False:
                Output.player[i].linearActuator.stop = True
                self.playerRemaningMoves[i] -= 1
    
    def evaluate(self, Input, Output):
        self.evaluateTime = time.time()
        for i in range(4):
            self.playerError[i] = abs(Input.player[i].linearActuator.currentPose - self.target)
            Output.player[i].linearActuator.stop = True

        
    
    def endRound(self, Input, Output):
        for i in range(4):
            Output.player[i].linearActuator.moveTo = self.target
        # should play audio to announce the winner of the round
            
    def endGame(self, Input, Output):
        i = self.playerScores.index(max(self.playerScores))
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