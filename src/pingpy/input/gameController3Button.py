from .gameController import GameControllerInput

class GameController3ButtonInput(GameControllerInput):
    def __init__(self):
        super().__init__()
        self.left=False
        self.right=False
        self.Shoot=False