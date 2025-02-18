from .gameMode import GameMode
from ..output.output import Output
from ..input.input import Input
import time
from pingpy.debug import logger
from pingpy.config.config import GREEN, ORANGE, RED, BLUE, PATH_AUDIO_123SOLEIL_INTRO, PATH_AUDIO_123SOLEIL_123, PATH_AUDIO_123SOLEIL_SOLEIL, PATH_AUDIO_GAGNE, PATH_AUDIO_PLAYER_BLEU, PATH_AUDIO_PLAYER_ROUGE, PATH_AUDIO_PLAYER_JAUNE, PATH_AUDIO_PLAYER_VERT, MAX_REACTION_TIME, WHITE, PATH_AUDIO_BEGIN_GAME
from random import uniform

class AutoPlayRedLightGreenLight:
    def __init__(self):
        self.loosingProb = 0
        self.minReactionTime = 0
        self.shouldStopDelay = None
        self.stopOrMoveSaid = None
        
    def set_skill(self, skill, reactionTime):
        self.loosingProb = 0.4*(1-skill) # give a random aspect
        self.minReactionTime = 0.5 * reactionTime 
        
    def randomize_duration(self, greenLightDuration, reactionTime, maxSpeed, accel):
        t0 = greenLightDuration + self.minReactionTime 
        t1 = (t0*self.loosingProb - greenLightDuration - reactionTime)/(self.loosingProb-1)  - maxSpeed/accel - 0.05 # 0.05s is transmission latency, in average
        self.shouldStopDelay = uniform(t0, t1)

    def run(self, playerInput, playerOutput, timeFromInitMatch):
        if self.shouldStopDelay is None :
            return None
        if timeFromInitMatch<self.shouldStopDelay:
            if self.stopOrMoveSaid is not True:
                playerOutput.linearActuator.setMaxSpeed = 10.0
                playerOutput.linearActuator.moveToRight = True
                self.stopOrMoveSaid = True
                return True
        else:
            if self.stopOrMoveSaid is not False:
                playerOutput.linearActuator.stop = True
                self.stopOrMoveSaid = False
                return False
        return None
        
        
        

class RedLightGreenLight(GameMode):
    """
    Game mode of Red Light Green Light. 1 2 3 soleil in French.
    """
    def __init__(self):
        self.standby = False
        self.isLightGreen = False
        self.timeInit = 0
        self.waitForStart = False
        self.waitForAudioEnd = False
        self.durationGreenLight = None  # Time of green light
        self.durationRedLight = None # Time of red light
        self.reactionTime = 0.3  # Time of reaction
        self.currentDifficulty = 0
        self.color = WHITE
        self.descriptionAudioPath = PATH_AUDIO_123SOLEIL_INTRO
        self.autoPlayer = [AutoPlayRedLightGreenLight() for _ in range(4)]
        self.playingSpeed = 10.0
        self.playingAccel = 200.0

        logger.write_in_log("INFO", __name__, "__init__", "Game mode initialized.")

    def setup(self, Input, Output):
        """
        Setup the game mode.
        """
        self.isLightGreen = False
        self.timeInit = 0
        self.waitForStart = False
        self.durationGreenLight = None  # Time of green light
        self.durationRedLight = None # Time of red light

        if not Input.player or not Output.player:
            logger.write_in_log("ERROR", __name__, "setup", "Input or Output data is missing.")
            return

        for i in range(4):
            try:
                playerOutput = Output.player[i]
                playerInput = Input.player[i]
                playerOutput.playerLedStrip.area = [-200, 200] 
                playerOutput.linearActuator.moveToLeft = True
                playerOutput.linearActuator.setMaxSpeed = 200.0
                playerOutput.linearActuator.setMaxAccel = self.playingAccel
                playerInput.gameController.inAction = None
        
            except IndexError:
                logger.write_in_log("ERROR", __name__, "setup", f"No output found for player ID {Input.playerInput[i]}.")

        self.timeInit = time.time()
        self.randomize_duration(Output)
        # Output.speaker.audioPiste = None 
        self.inGame = True
        self.waitForStart = True
        self.standby = False
        # Output.speaker.stop = True

        logger.write_in_log("INFO", __name__, "setup", "Setup complete.")
    
    def wait_for_start(self, input : Input, output: Output)->bool:
        """
        Args:
            Input (_type_): refer to input/input.py
            output (_type_): refer to output/output.py

        Returns:
            bool : True if the game should waiting before start, False otherwise
        """
        if not self.waitForStart:
            return True
        for player in input.player:
            if player.linearActuator.currentPose is None or player.linearActuator.leftLimit is None:
                return False
            elif player.linearActuator.currentPose < player.linearActuator.leftLimit - 1e-3:
                # wait until all players are at the left limit
                return False
        
        self.waitForStart = False
        output.speaker.audioPiste = PATH_AUDIO_BEGIN_GAME
        self.waitForAudioEnd = True
        return True
    
    def randomize_duration(self, Output):
        """
        Randomize the duration of the green and red lights.
        """
        try:
            min_duration = Output.speaker.duration(PATH_AUDIO_123SOLEIL_123)+Output.speaker.duration(PATH_AUDIO_123SOLEIL_SOLEIL) + 0.5
            max_duration = min_duration + 3
            self.durationGreenLight = uniform(min_duration, max_duration)
            self.durationRedLight = uniform(2 * self.reactionTime, max_duration)
            for playerId in range(4):
                self.autoPlayer[playerId].randomize_duration(self.durationGreenLight, self.reactionTime, self.playingSpeed, self.playingAccel)
                self.autoPlayer[playerId].set_skill(self.currentDifficulty, self.reactionTime)
            # logger.write_in_log("INFO", __name__, "randomize_duration", f"Green light duration: {self.durationGreenLight}, Red light duration: {self.durationRedLight}")
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "randomize_duration", f"Failed to randomize durations: {e}")

    def can_move(self, currentTime):
        """
        Check if the player can move (green light).
        """
        elapsedTime = currentTime - self.timeInit
        if elapsedTime < 0:
            logger.write_in_log("ERROR", __name__, "can_move", "Elapsed time is negative.")
        result = elapsedTime < self.durationGreenLight + self.reactionTime
        return result

    def check_action(self, playerInput, playerOutput, currentTime, playerId):
        """
        Checks the player's action according to the current state of the light.
        """
        
        
        try : 
            if playerOutput.playerRunningRedLightAt is not None:
                if playerInput.linearActuator.currentPose is not None :
                    if playerInput.linearActuator.currentPose > playerInput.linearActuator.leftLimit + 1e-3:
                        playerOutput.playerRunningRedLightAt = None
        except Exception as e:
            pass
        
        canmove = self.can_move(currentTime)   
        action = playerInput.gameController.inAction
        if playerInput.auto.mode == True:
            action = self.autoPlayer[playerId].run(playerInput, playerOutput, currentTime - self.timeInit)

        if action is None:
            if not canmove and playerInput.linearActuator.moving :
                self.lose(playerOutput)
            return
        
        if action:
            if canmove:
                playerOutput.linearActuator.setMaxSpeed = self.playingSpeed
                playerOutput.linearActuator.moveToRight = True
            else:
                self.lose(playerOutput)  
        else:
            if not canmove and playerInput.linearActuator.moving :
                self.lose(playerOutput)
            else:
                playerOutput.linearActuator.stop = True
             
        playerInput.gameController.inAction = None
            
        
        
    def lose(self, playerOutput):
        """
        Handles the player's loss by moving them back to the left at speed 200.
        """
        playerOutput.linearActuator.moveToLeft = True
        playerOutput.linearActuator.setMaxSpeed = self.playingAccel
        playerOutput.playerRunningRedLightAt = time.time()
        playerOutput.playerLedStrip.color = ORANGE

        logger.write_in_log("INFO", __name__, "lose", "Player {playerOutput.playerID} lost.")
        

    def check_victory(self, playerInput, playerOutput):
        """
        Checks if a player has won.
        """
        if playerInput.linearActuator.currentPose is None :
            return False
        
        if playerInput.linearActuator.rightLimit is None :
            logger.write_in_log("ERROR", __name__, "check_victory", "Right limit is not set.")
            return False
        
        if playerInput.linearActuator.currentPose <= playerInput.linearActuator.rightLimit + 1:
            playerOutput.isWinner = True
            return True
        
        return False

    def cycle(self, currentTime, Output, playerId):
        """
        Manages the alternation between green and red lights.
        """
        elapsedTime = currentTime - self.timeInit
        try:
            if elapsedTime < self.durationGreenLight:
                if not self.isLightGreen:
                    self.isLightGreen = True
                    # logger.write_in_log("INFO", __name__, "cycle", "Green light.")
                    Output.speaker.audioPiste = [PATH_AUDIO_123SOLEIL_123]
                    for PlayerOutput in Output.player:
                        PlayerOutput.playerLedStrip.color = GREEN
            elif elapsedTime < self.durationGreenLight + self.durationRedLight:
                if self.isLightGreen:
                    self.isLightGreen = False
                    # logger.write_in_log("INFO", __name__, "cycle", "Red light.")
                    for PlayerOutput in Output.player:
                        PlayerOutput.playerLedStrip.color = RED
                    Output.speaker.audioPiste = [PATH_AUDIO_123SOLEIL_SOLEIL]
            else:
                self.timeInit = currentTime
                self.randomize_duration(Output)
                
                
        except Exception as e:
            logger.write_in_log("ERROR", "RedLightGreenLight", "cycle", f"Cycle error: {e}")

    def compute(self, Input, Output):
        """
        Executes an iteration of the game mode.
        """
        if(not self.wait_for_start(Input, Output)):
            return
        
        if self.waitForAudioEnd:
            if Output.speaker.isBusy:
                return
            self.waitForAudioEnd = False
            
        if Input.UICorner.resetShortPress:
            Input.UICorner.resetShortPress = None
            self.setup(Input, Output)
            return
            
        if self.standby:
            return
        
        if not Input.player:
            logger.write_in_log("ERROR", __name__, "compute", "No players connected...")
            return
        
        if not self.inGame:
            return
        
        if Input.UICorner.level is not None:
            self.currentDifficulty = Input.UICorner.level
            self.reactionTime = MAX_REACTION_TIME*(1-(Input.UICorner.level))
            Input.UICorner.level = None
            logger.write_in_log("INFO", __name__, f"compute, select reactionTime {self.reactionTime}s")

        for i in range(4):
            if self.check_victory(Input.player[i], Output.player[i]):
                self.makeWin(Output, i)
                break
            else:
                self.check_action(Input.player[i], Output.player[i], time.time(),i)
                self.cycle(time.time(), Output, i)

    def makeWin(self, Output, winnerID):
        """
        Makes a player win the game.
        """
        for i in range(4):
            if i == winnerID:
                Output.player[i].playerLedStrip.color = GREEN
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
                
            else:
                Output.player[i].playerLedStrip.color = RED
            Output.player[i].linearActuator.stop = True
        Output.speaker.stop = True
        self.standby = True
                

    def stop(self, output_ptr):
        """
        Stops the game and resets the outputs.
        """
        self.inGame = False
        
        for i in range(4):
            output_ptr.player[i].linearActuator.stop = True
            
        logger.write_in_log("INFO", __name__, "stop", "Game stopped.")
