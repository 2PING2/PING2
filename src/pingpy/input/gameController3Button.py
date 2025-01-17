from .gameController import GameControllerInput
from pingpy.debug import logger

class GameController3ButtonInput(GameControllerInput):
    def __init__(self):
        super().__init__()
        self.left=False
        self.right=False
        self.shoot=False
        self.countButton=0 ##Penser a reinitialiser a chaque mode de jeu
        logger.write_in_log("INFO", __name__, "__init__")