from pingpy.debug import logger

class LinearActuatorOutput:
    def __init__(self):
        self.moveToRight = None
        self.moveToLeft = None
        self.moveTo = None
        self.setSpeed = None
        self.stop = None
        logger.write_in_log("INFO", __name__, "__init__")
    pass