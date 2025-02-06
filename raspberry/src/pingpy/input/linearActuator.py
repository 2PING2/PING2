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
        
    def computeInterpolation(self, timeStep, i = 0):
        if self.currentAcceleration is None:
            return
        if i == 0:
            print("current acceleration", self.currentAcceleration)
            print("current speed", self.currentSpeed)
            print("current pose", self.currentPose)
        if self.currentSpeed is None:
            return
        self.currentSpeed += self.currentAcceleration * timeStep
        if self.currentPose is None:
            return
        self.currentPose += self.currentSpeed * timeStep
                  