from pingpy.debug import logger

class PlayerLedStripOutput:
    def __init__(self):
        self.leftLimit=200
        self.rightLimit=-200
        self.area = None
        self.color = None
        logger.write_in_log("INFO", __name__, "__init__")