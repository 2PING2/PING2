from pingpy.debug import logger

class LinearActuatorOutput:
    def __init__(self):
        self.moveToRight = None
        self.moveToLeft = None
        self.moveTo = None
        self.setMaxSpeed = None
        self.stop = None
        self.setMaxAccel = None
        self.preventWhenMoveEnded = None
        self.askForCalibration = None
        logger.write_in_log("INFO", __name__, "__init__")
    pass