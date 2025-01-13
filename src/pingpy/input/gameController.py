from pingpy.debug import logger

class GameControllerInput():
    def __init__(self):
        self.inAction = False
        logger.write_in_log("INFO", __name__, "__init__")
