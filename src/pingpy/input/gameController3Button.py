from .gameController import GameControllerInput
from pingpy.debug import logger

class GameController3ButtonInput(GameControllerInput):
    def __init__(self):
        super().__init__()
        self.left=False
        self.right=False
        self.Shoot=False
        logger.write_in_log("INFO", __name__, "__init__")