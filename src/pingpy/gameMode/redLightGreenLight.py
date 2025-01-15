from .gameMode import GameMode
from ..output.output import Output
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
        self.lastPlayedAudio = None

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
            playerOutput = self.outputData.ListPlayerOutput[playerInput.idPlayer]
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

        if elapsedTime < 0:
            logger.write_in_log("ERROR", "gameMode", "cycle", "elapsed time has a negative value")

        # Changer la durée du feu vert à chaque cycle
        if elapsedTime >= self.durationGreenLight + self.reactionTime:  # Fin d'un cycle
            self.timeInit = currentTime
            self.isLightGreen = True

            
            min_duration = (
                Output.Speaker.duration("123Soleil.wav") +
                Output.Speaker.duration("Soleil.wav") +
                1  
            )
            max_duration = 10  
            self.durationGreenLight = random.uniform(min_duration, max_duration)

            
            self.lastPlayedAudio = None
        else:
            self.isLightGreen = elapsedTime < self.durationGreenLight

        
        if elapsedTime <= self.durationGreenLight + self.reactionTime:
            if elapsedTime <= self.durationGreenLight:
                if self.lastPlayedAudio != "123Soleil.wav":
                    Output.Speaker.audioPiste = "123Soleil.wav"
                    self.lastPlayedAudio = "123Soleil.wav"
            elif elapsedTime <= self.durationGreenLight - Output.Speaker.duration("123Soleil.wav"):
                if self.lastPlayedAudio != "Soleil.wav":
                    Output.Speaker.audioPiste = "Soleil.wav"
                    self.lastPlayedAudio = "Soleil.wav"
            Output.LedStrip.color(GREEN)
        elif elapsedTime < 2 * self.durationGreenLight + self.reactionTime:
            Output.LedStrip.color(RED)
            self.isLightGreen = False

    def compute(self, Input, Output):
        """
        Executes an iteration of the game mode.
        """
        # Cycle between green and red light
        self.cycle(time.time(), Output)
        if not Input.ListPlayerInput:
            logger.write_in_log("ERROR", "gameMode", "run", "no player was connected")
        for playerInput in Input.ListPlayerInput:
            if self.check_victory(playerInput):
                self.stop(Input)
                return Output

            # check player action
            self.check_action(playerInput, time.time())


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
