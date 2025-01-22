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
        self.durationGreenLight = None  # Temps du feu vert
        self.durationRedLight = None
        self.reactionTime = 0.5  # Temps de réaction
        self.color = GREEN

        self.initialized = False
        logger.write_in_log("INFO", "RedLightGreenLight", "__init__", "Game mode initialized.")

    def setup(self, Input, Output):
        """
        Setup the game mode.
        """
        logger.write_in_log("INFO", "RedLightGreenLight", "setup", "Setting up the game mode.")

        if not Input.player or not Output.player:
            logger.write_in_log("ERROR", "RedLightGreenLight", "setup", "Input or Output data is missing.")
            return

        Output.speaker.audioPiste = r"audio\redLightGreenLight\Intro_123Soleil.wav"
        for i in range(1,2):
            try:
                playerOutput = Output.player[i]
                playerOutput.playerLedStrip.area = [-200, 200]
                playerOutput.playerLedStrip.color =  GREEN
                playerOutput.linearActuator.move_to = Input.player[i].linearActuator.leftLimit
            except IndexError:
                logger.write_in_log("ERROR", "RedLightGreenLight", "setup", f"No output found for player ID {Input.playerInput[i]}.")

        self.timeInit = time.time()
        self.isLightGreen = True
        self.randomize_duration()
        Output.speaker.audioPiste = None

        logger.write_in_log("INFO", "RedLightGreenLight", "setup", "Setup complete.")

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
            logger.write_in_log("DEBUG", "RedLightGreenLight", "randomize_duration", 
                                 f"GreenLight: {self.durationGreenLight}s, RedLight: {self.durationRedLight}s")
        except Exception as e:
            logger.write_in_log("ERROR", "RedLightGreenLight", "randomize_duration", f"Failed to randomize durations: {e}")

    def can_move(self, currentTime):
        """
        Check if the player can move (green light).
        """
        elapsedTime = currentTime - self.timeInit
        if elapsedTime < 0:
            logger.write_in_log("ERROR", "RedLightGreenLight", "can_move", "Elapsed time is negative.")
        result = elapsedTime < self.durationGreenLight + self.reactionTime
        logger.write_in_log("DEBUG", "RedLightGreenLight", "can_move", f"ElapsedTime: {elapsedTime}, CanMove: {result}")
        return result

    def check_action(self, playerInput, playerOutput, currentTime):
        """
        Checks the player's action according to the current state of the light.
        """
        if playerInput.gameController.inAction:
            if self.can_move(currentTime):
                playerOutput.linearActuator.moveToRight = True
                playerOutput.playerLedStrip.color = GREEN
                logger.write_in_log("INFO", "RedLightGreenLight", "check_action", f"Player moved during green light.")
            else:
                playerOutput.linearActuator.moveTo = playerInput.linearActuator.leftLimit
                playerOutput.linearActuator.moveToRight = False
                playerOutput.playerLedStrip.color = ORANGE
                logger.write_in_log("WARNING", "RedLightGreenLight", "check_action", f"Player moved during red light.")
        else:
            playerOutput.linearActuator.moveToRight = False

    def check_victory(self, playerInput, playerOutput):
        """
        Checks if a player has won.
        """
        if playerInput.linearActuator.currentPose is None or playerInput.linearActuator.rightLimit is None:
            return False
        if playerInput.linearActuator.currentPose >= playerInput.linearActuator.rightLimit:
            playerOutput.playerLedStrip.color = YELLOW
            return True
        return False

    def cycle(self, currentTime, Output):
        """
        Manages the alternation between green and red lights.
        """
        elapsedTime = currentTime - self.timeInit
        try:
            if elapsedTime <= self.durationGreenLight:
                if elapsedTime < self.durationGreenLight - Output.speaker.duration("Soleil.wav"):
                    Output.speaker.audioPiste = r"audio\redLightGreenLight\123.wav"
                else:
                    Output.speaker.audioPiste = r"audio\redLightGreenLight\Soleil.wav"
                for PlayerOutput in Output.player:
                    PlayerOutput.playerLedStrip.color = GREEN
                self.isLightGreen = True
            elif elapsedTime < self.durationGreenLight + self.durationRedLight:
                for PlayerOutput in Output.player:
                    PlayerOutput.playerLedStrip.color = RED
                self.isLightGreen = False
            else:
                self.timeInit = currentTime
                self.isLightGreen = True
                self.randomize_duration()
            logger.write_in_log("DEBUG", "RedLightGreenLight", "cycle", f"ElapsedTime: {elapsedTime}, LightGreen: {self.isLightGreen}")
        except Exception as e:
            logger.write_in_log("ERROR", "RedLightGreenLight", "cycle", f"Cycle error: {e}")

    def compute(self, Input, Output):
        """
        Executes an iteration of the game mode.
        """
        if not self.initialized:
            self.setup(Input, Output)
            self.initialized = True

        if not Input.player:
            logger.write_in_log("ERROR", "RedLightGreenLight", "compute", "No players connected.")
            return

        for i in range(1,2):
            if self.check_victory(Input.player[i], Output.player[i]):
                self.stop(Input, Output)
            else:
                self.check_action(Input.player[i], Output.player[i], time.time())
        self.cycle(time.time(), Output)

    def stop(self, Input, Output):
        """
        Stops the game and resets the outputs.
        """
        for i in range(1,2):
            playerOutput = Output.player[i]
            playerOutput.playerLedStrip.color(None)
            playerOutput.linearActuator.moveToRight = False
