from .gameMode import GameMode
from ..output.output import Output
import time
from pingpy.debug import logger
from pingpy.config.config import GREEN, ORANGE, YELLOW, RED
from random import uniform

class RedLightGreenLight(GameMode):
    """
    Game mode of Red Light Green Light. 1 2 3 soleil in French.
    """
    def __init__(self):
        self.isLightGreen = False
        self.timeInit = 0
        self.waitForStart = False
        self.durationGreenLight = None  # Time of green light
        self.durationRedLight = None # Time of red light
        self.reactionTime = 0.5  # Time of reaction
        self.color = GREEN
        self.descriptionAudioPath = r"raspberry/src/pingpy/audio/redLightGreenLight/Intro_123SOLEIL.wav"

        # self.initialized = False
        logger.write_in_log("INFO", __name__, "__init__", "Game mode initialized.")

    def setup(self, Input, Output):
        """
        Setup the game mode.
        """
        if not Input.player or not Output.player:
            logger.write_in_log("ERROR", __name__, "setup", "Input or Output data is missing.")
            return

        # Output.speaker.audioPiste = r"audio/redLightGreenLight/Intro_123Soleil.wav"
        
        for i in range(4):
            try:
                playerOutput = Output.player[i]
                playerInput = Input.player[i]
                playerOutput.playerLedStrip.area = [-200, 200] 
                playerOutput.playerLedStrip.color =  GREEN
                playerOutput.linearActuator.moveToLeft = True
                playerOutput.linearActuator.setMaxSpeed = 250.0
                playerOutput.linearActuator.setMaxAccel = 200.0
                playerInput.gameController.inAction = None
                
                # demander et attendre la réponse du joueur
        
            except IndexError:
                logger.write_in_log("ERROR", __name__, "setup", f"No output found for player ID {Input.playerInput[i]}.")

        self.timeInit = time.time()
        self.isLightGreen = True 
        self.randomize_duration() 
        Output.speaker.audioPiste = None 
        self.inGame = True
        self.waitForStart = True

        logger.write_in_log("INFO", __name__, "setup", "Setup complete.")
    
    def wait_for_start(self, Input, Output):
        if not self.waitForStart:
            return True
        for player in Input.player:
            if player.linearActuator.currentPose is None or player.linearActuator.leftLimit is None:
                return False
            elif player.linearActuator.currentPose < player.linearActuator.leftLimit - 1e-3:
                return False
        
        self.waitForStart = False
        return True
    
    def randomize_duration(self):
        """
        Randomize the duration of the green and red lights.
        """
        try:
            #min_duration = (
            #    Output.speaker.duration(r"audio\redLightGreenLight\123.wav") +
            #    Output.speaker.duration(r"audio\redLightGreenLight\Soleil.wav") +
            #    1
            #)
            min_duration = ( 1 + 1 + 1 )
            max_duration = 10
            self.durationGreenLight = uniform(min_duration, max_duration)
            self.durationRedLight = uniform(2 * self.reactionTime, max_duration)
            # logger.write_in_log("DEBUG", __name__, "randomize_duration", f"GreenLight: {self.durationGreenLight}s, RedLight: {self.durationRedLight}s")
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

    def check_action(self, playerInput, playerOutput, currentTime):
        """
        Checks the player's action according to the current state of the light.
        """
        try : 
            if playerOutput.playerRunningRedLightAt is not None:
                return
        except Exception as e:
            pass
        
        canmove = self.can_move(currentTime)
        if canmove:
            playerOutput.playerLedStrip.color = GREEN # 1 2 3 soleil
        else:
            playerOutput.playerLedStrip.color = RED # red light
        
        if playerInput.gameController.inAction is None:
            if not canmove and playerInput.linearActuator.moving:
                self.lose(playerOutput)
            return
        
        if playerInput.gameController.inAction:
            if canmove:
                playerOutput.linearActuator.setMaxSpeed = 10.0
                playerOutput.linearActuator.moveToRight = True
            else:
                self.lose(playerOutput)  
        else:
            if not canmove and playerInput.linearActuator.moving:
                self.lose(playerOutput)
            else:
                playerOutput.linearActuator.stop = True
             
        playerInput.gameController.inAction = None
            
        
        
    def lose(self, playerOutput):
        """
        Handles the player's loss by moving them back to the left at speed 200.
        """
        playerOutput.linearActuator.moveToLeft = True
        playerOutput.linearActuator.setMaxSpeed = 250.0
        playerOutput.playerLedStrip.color = ORANGE
        playerOutput.playerRunningRedLightAt = time.time()
        logger.write_in_log("INFO", __name__, "lose", "Player has lost.")
        

    def check_victory(self, playerInput, playerOutput):
        """
        Checks if a player has won.
        """
        if playerInput.linearActuator.currentPose is None :
            return False
        
        if playerInput.linearActuator.rightLimit is None :
            logger.write_in_log("ERROR", __name__, "check_victory", "Right limit is not set.")
            return False
        
        if playerInput.linearActuator.currentPose <= playerInput.linearActuator.rightLimit + 1e-3:
        # if playerInput.linearActuator.currentPose <= 100 + 1e-3:
            playerOutput.isWinner = True
            playerOutput.playerLedStrip.color = YELLOW
            return True
        
        return False

    def cycle(self, currentTime, Output):
        """
        Manages the alternation between green and red lights.
        """
        elapsedTime = currentTime - self.timeInit
        try:
            if elapsedTime < self.durationGreenLight:
                if not self.isLightGreen:
                    self.isLightGreen = True
                    Output.speaker.audioPiste = r"raspberry/src/pingpy/audio/redLightGreenLight/123.wav"
                    for PlayerOutput in Output.player:
                        PlayerOutput.playerLedStrip.color = GREEN
            elif elapsedTime < self.durationGreenLight + self.durationRedLight:
                if self.isLightGreen:
                    self.isLightGreen = False
                    for PlayerOutput in Output.player:
                        PlayerOutput.playerLedStrip.color = RED
                    Output.speaker.audioPiste = r"raspberry/src/pingpy/audio/redLightGreenLight/Soleil.wav"
            else:
                self.timeInit = currentTime
                self.randomize_duration()
            for playerOutput in Output.player:
                try:
                    if playerOutput.playerRunningRedLightAt is None:
                        continue
                    if currentTime - playerOutput.playerRunningRedLightAt < 2:
                        playerOutput.playerLedStrip.color = ORANGE
                    else:
                        playerOutput.playerRunningRedLightAt = None
                except Exception as e:
                    pass
                
        except Exception as e:
            logger.write_in_log("ERROR", "RedLightGreenLight", "cycle", f"Cycle error: {e}")

    def compute(self, Input, Output):
        """
        Executes an iteration of the game mode.
        """
        if(not self.wait_for_start(Input, Output)):
            return
        
        if not Input.player:
            logger.write_in_log("ERROR", "RedLightGreenLight", "compute", "No players connected.")
            return
        
        if not self.inGame:
            return

        for i in range(4):
            if self.check_victory(Input.player[i], Output.player[i]):
                self.stop(Input, Output, i) 
                break
            else:
                self.check_action(Input.player[i], Output.player[i], time.time())
                self.cycle(time.time(), Output)

        

    def stop(self, Input, Output, winnerID):
        """
        Stops the game and resets the outputs.
        """
        logger.write_in_log("INFO", __name__, "stop", "Game stopped, winner is player " + str(winnerID + 1))
        self.inGame = False
        for i in range(4):
            if i == winnerID:
                Output.player[i].playerLedStrip.color = GREEN
            else:
                Output.player[i].playerLedStrip.color = RED
                
            Output.player[i].linearActuator.stop = True