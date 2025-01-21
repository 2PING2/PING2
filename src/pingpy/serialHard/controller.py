from .serialCom import SerialCom
from ..input.gameController import GameControllerInput
from pingpy.debug import logger

from pingpy.config.config import PUSH_KEY, RELEASE_KEY, LEFT_BUTTON_KEY, RIGHT_BUTTON_KEY, SHOOT_BUTTON_KEY, SEP_KEY

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
        new_line = new_line.split(SEP_KEY)
        button = new_line[0]
        if new_line[1] == PUSH_KEY:
            self.controllerInput.inAction = True
            if button == LEFT_BUTTON_KEY:
                self.controllerInput.left = True
            if button == RIGHT_BUTTON_KEY:
                self.controllerInput.right = True
            if button == SHOOT_BUTTON_KEY:
                self.controllerInput.shoot = True
        elif new_line[1] == RELEASE_KEY:
            self.controllerInput.inAction = False
            if button == LEFT_BUTTON_KEY:
                self.controllerInput.left = False
            if button == RIGHT_BUTTON_KEY:
                self.controllerInput.right = False
            if button == SHOOT_BUTTON_KEY:
                self.controllerInput.shoot = False
