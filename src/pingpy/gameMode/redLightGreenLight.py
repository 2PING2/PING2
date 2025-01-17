from pingpy.gameMode import GameMode
from pingpy.output import Output
import time
from pingpy.debug import logger
from pingpy.config.config import GREEN, ORANGE, YELLOW, RED
from random import random


class RedLightGreenLight(GameMode):
    """
    Game mode of Red Light Green Light. 1 2 3 soleil in French.
    """
    def __init__(self):
        self.isLightGreen = False
        self.timeInit = 0
        self.durationGreenLight = None  # Temps du feu vert
        self.reactionTime = 0.5      # Temps de réaction

        self.initialized = False

    def setup(self, Input, Output):
        """
        Setup the game mode.
        """
        for playerInput in Input.ListPlayerInput:
            playerOutput = Output.ListPlayerOutput[playerInput.idPlayer]
            playerOutput.PlayerLedStrip.color(GREEN)
            playerOutput.LinearActuatorOutput.move_to = Input.ListPlayerInput[playerInput.idPlayer].LinearActuatorInput.leftLimit
        self.timeInit = time.time()

        self.isLightGreen = True
        self.randomize_duration()
        Output.Speaker.audioPiste = None
        
    def randomize_duration(self):
        """
        Randomize the duration of the green light.
        """
        min_duration = (
                Output.Speaker.duration("123Soleil.wav") +
                Output.Speaker.duration("Soleil.wav") +
                1  
            )
        max_duration = 10  
        self.durationGreenLight = random.uniform(min_duration, max_duration)

    def can_move(self, currentTime):
        """
        Check if the player can move (green light)
        """
        elapsedTime = currentTime - self.timeInit
        if elapsedTime<0:
            logger.write_in_log("ERROR", "gameMode", "can_move", "elaspsed time has a negative value")
        return elapsedTime < self.durationGreenLight + self.reactionTime

    def check_action(self, playerInput, playerOutput, currentTime):
        """
        Checks the player's action according to the current state of the light.
        """
        if playerInput.GameController.newAction:
            playerOutput = self.outputData.ListPlayerOutput[playerInput.idPlayer]
            
            if self.can_move(currentTime):
                # light green : allowed to move
                playerOutput.LinearActuatorOutput.move_to_right = True
                playerOutput.LinearActuatorOutput.move_to_leftLimit = False
                #playerOutput.PlayerLedStrip.onPlayer(GREEN)
                playerOutput.PlayerLedStrip.color(GREEN)
            else:
                # light red : not allowed to move
                playerOutput.LinearActuatorOutput.move_to_leftLimit = True
                #playerOutput.PlayerLedStrip.onPlayer(ORANGE)
                playerOutput.PlayerLedStrip.color(ORANGE)
        else:
            playerOutput = Output.ListPlayerOutput[playerInput.idPlayer]
            playerOutput.LinearActuatorOutput.move_to_right = False
            playerOutput.LinearActuatorOutput.move_to_leftLimit = False

    def check_victory(self, playerInput, Output):
        """
        Checks if a player has won.
        """
        if playerInput.LinearActuatorInput.currentPose >= playerInput.LinearActuatorInput.rightLimit-20:
            Output.ListPlayerOutput[playerInput.idPlayer].PlayerLedStrip.onPlayer(YELLOW)
            return True
        return False

    import random

    def cycle(self, currentTime, Output):
        """
        Manages the alternation between green and red lights.
        """
        elapsedTime = currentTime - self.timeInit

        if elapsedTime <= self.durationGreenLight:
            if elapsedTime < 0:
                logger.write_in_log("ERROR", "gameMode", "cycle", "elapsed time has a negative value")
            elif elapsedTime < self.durationGreenLight - Output.Speaker.duration("Soleil.wav"):
                if Output.Speaker.audioPiste != "123.wav":
                    Output.Speaker.audioPiste = "123.wav"

            else:
                if self.audioPiste != "Soleil.wav":
                    Output.Speaker.audioPiste = "Soleil.wav"
                
            Output.LedStrip.color(GREEN)
            self.isLightGreen = True

        elif elapsedTime > self.durationGreenLight and elapsedTime < 2 * self.durationGreenLight + self.reactionTime:
            Output.LedStrip.color(RED)
            self.isLightGreen = False
        else:
            self.timeInit = currentTime
            self.isLightGreen = True
            self.randomize_duration()



    def compute(self, Input, Output):
        """
        Executes an iteration of the game mode.
        """
        if self.initialized == False:
            self.setup(Input, Output)
            self.initialized = True

        if not Input.ListPlayerInput:
            logger.write_in_log("ERROR", "gameMode", "run", "no player was connected")
        for playerInput in Input.ListPlayerInput:
            if self.check_victory(playerInput, Output):
                self.stop(Input, Output)
                
            # check player action
            self.check_action(playerInput, Output.ListPlayerOutput[playerInput.idPlayer], time.time())
        self.cycle(time.time(), Output)

    def stop(self, Input, Output):
        """
        Stops the game and resets the outputs.
        """
        for playerInput in Input.ListPlayerInput:
            playerOutput = Output.ListPlayerOutput[playerInput.idPlayer]
            #playerOutput.PlayerLedStrip.clearPlayer()
            playerOutput.PlayerLedStrip.color(None)
            playerOutput.LinearActuatorOutput.move_to_right = False
            #playerOutput.LinearActuatorOutput.move_to_leftLimit = True
            playerOutput.LinearActuatorOutput.move_to = Input.ListPlayerInput[playerInput.idPlayer].LinearActuatorInput.leftLimit
