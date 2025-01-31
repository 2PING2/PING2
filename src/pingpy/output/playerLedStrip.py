from pingpy.debug import logger

class PlayerLedStripOutput:
    def __init__(self):
        self.area = None
        self.color = None
        self.brightness = None
        logger.write_in_log("INFO", __name__, "__init__")