from .serialCom import SerialCom
from ..input.gameController import GameControllerInput
from pingpy.debug import logger

from pingpy.config.config import PUSH_KEY, RELEASE_KEY

class ControllerSerial(SerialCom):
    def __init__(self, controllerInput, port, baudrate, timeout):
        super().__init__(port, baudrate, timeout)
        self.controllerInput = controllerInput
        logger.write_in_log("INFO", __name__, "__init__")
        
    def read(self, input_ptr):
        self.read_data_task()
        new_line = self.consume_older_data()
        if new_line is None:
            return
        new_line = new_line.split('/')
        if new_line[0] == PUSH_KEY:
            self.controllerInput.inAction = True
        elif new_line[0] == RELEASE_KEY:
            self.controllerInput.inAction = False