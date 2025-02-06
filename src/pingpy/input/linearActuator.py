from pingpy.debug import logger

class LinearActuatorInput():
    def __init__(self):
        self.moving = False
        self.leftLimit= None
        self.rightLimit= None
        self.currentPose= None
        self.currentSpeed= None
        self.currentAcceleration= None
        logger.write_in_log("INFO", __name__, "__init__")
        
    def computeInterpolation(self, timeStep):
        if self.currentAcceleration is None:
            return
        print("current acceleration", self.currentAcceleration)
        if self.currentSpeed is None:
            return
        print("current speed", self.currentSpeed)
        self.currentSpeed += self.currentAcceleration * timeStep
        if self.currentPose is None:
            return
        print("current pose", self.currentPose)
        self.currentPose += self.currentSpeed * timeStep
                  