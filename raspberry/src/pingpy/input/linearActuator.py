from pingpy.debug import logger

class LinearActuatorInput():
    def __init__(self):
        self.moving = False
        self.leftLimit= None
        self.rightLimit= None
        self.currentPose= None
        logger.write_in_log("INFO", __name__, "__init__")