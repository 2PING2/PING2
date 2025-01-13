from .serialCom import SerialCom
from ..input.gameController import GameControllerInput

from pingpy.config.config import PUSH_KEY, RELEASE_KEY

class ControllerSerial(SerialCom):
    def __init__(self, controllerInput, port, baudrate, timeout):
        super().__init__(port, baudrate, timeout)
        self.controllerInput = controllerInput
        print("ControllerSerial class created")
        
    def process_data(self):
        
        if self.lastData[1] == PUSH_KEY:
            self.controllerInput.inAction = True
        elif self.lastData[1] == RELEASE_KEY:
            self.controllerInput.inAction = False