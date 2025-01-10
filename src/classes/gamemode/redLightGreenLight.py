from classes.gameMode import GameMode
from classes.output.output import Output
import time
from logFile import*   
from classes.config import GREEN, ORANGE, YELLOW, RED
log = LogFile()

class RedLightGreenLight(GameMode):
    """
    Game mode of Red Light Green Light. 1 2 3 soleil in French.
    """
    def __init__(self):
        self.isLightGreen = False
        self.timeInit = time.time()
        self.durationGreenLight = 3  # Temps du feu vert
        self.reactionTime = 0.5      # Temps de réaction
        self.outputData = Output([])  # Initialisation des données de sortie

    def can_move(self, currentTime):
        """
        Check if the player can move (green light)
        """
        elapsedTime = currentTime - self.timeInit
        if elapsedTime<0:
            log.write_in_log("ERROR", "gameMode", "can_move", "elaspsed time has a negative value")
        return elapsedTime < self.durationGreenLight

    def check_action(self, playerInput, currentTime):
        """
        Checks the player's action according to the current state of the light.
        """
        if playerInput.GameController.newAction:
            playerOutput = self.outputData.ListPlayerOutput[playerInput.idPlayer]
            
            if self.can_move(currentTime):
                # light green : allowed to move
                playerOutput.LinearActuatorOutput.move_to_right = True
                playerOutput.LinearActuatorOutput.move_to_leftLimit = False
                playerOutput.PlayerLedStrip.onPlayer(GREEN)
            else:
                # light red : not allowed to move
                playerOutput.LinearActuatorOutput.move_to_leftLimit = True
                playerOutput.PlayerLedStrip.onPlayer(ORANGE)
        else:
            playerOutput = self.outputData.ListPlayerOutput[playerInput.idPlayer]
            playerOutput.LinearActuatorOutput.move_to_right = False
            playerOutput.LinearActuatorOutput.move_to_leftLimit = False

    def check_victory(self, playerInput):
        """
        Checks if a player has won.
        """
        if playerInput.LinearActuatorInput.currentPose >= playerInput.LinearActuatorInput.rightLimit-20:
            self.outputData.ListPlayerOutput[playerInput.idPlayer].PlayerLedStrip.onPlayer(YELLOW)
            return True
        return False

    def cycle(self, currentTime):
        """
        Manages the alternation between green and red lights.
        """
        elapsedTime = currentTime - self.timeInit
        if elapsedTime<0:
            log.write_in_log("ERROR", "gameMode", "cycle", "elaspsed time has a negative value")
        self.isLightGreen = elapsedTime < self.durationGreenLight

        self.outputData.ledStrip.onLedStrip(GREEN) if self.isLightGreen else self.outputData.ledStrip.onLedStrip(RED)

    def run(self, inputData):
        """
        Executes an iteration of the game mode.
        """
        if not inputData.ListPlayerInput:
            log.write_in_log("ERROR", "gameMode", "run", "no player was connected")
        for playerInput in inputData.ListPlayerInput:
            if self.check_victory(playerInput):
                self.stop(inputData)
                return self.outputData

            # check player action
            self.check_action(playerInput, time.time())

        # Cycle between green and red light
        self.cycle(time.time())
        return self.outputData

    def stop(self, inputData):
        """
        Stops the game and resets the outputs.
        """
        for playerInput in inputData.ListPlayerInput:
            playerOutput = self.outputData.ListPlayerOutput[playerInput.idPlayer]
            playerOutput.PlayerLedStrip.clearPlayer()
            playerOutput.LinearActuatorOutput.move_to_right = False
            playerOutput.LinearActuatorOutput.move_to_leftLimit = True
