from .serialCom import SerialCom
from ..input.gameController import GameControllerInput
from pingpy.debug import logger

from pingpy.config.config import PUSH_KEY, RELEASE_KEY, LEFT_BUTTON_KEY, RIGHT_BUTTON_KEY, SHOOT_BUTTON_KEY, SEP_KEY

class ControllerSerial(SerialCom):
    def __init__(self, controllerInput, port, baudrate, timeout):
        super().__init__(port, baudrate, timeout)
        self.controllerInput = controllerInput
        logger.write_in_log("INFO", __name__, "__init__")
        
    def stopOnDisconnect(self,controllerOutput):
        controllerOutput.stop = True
        
    def read(self, controllerInput, controllerOutput=None):
        try:
            self.read_data_task(self.stopOnDisconnect, controllerInput)
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read", f"Error in read_data_task: {e}")
            return
        try:
            new_line = self.consume_older_data()
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read", f"Error in consume_older_data: {e}")
            return
        if new_line is None:
            return
        try:
            new_line = new_line.split(SEP_KEY)
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read", f"Error in split: {e}")
            return
        if len(new_line) < 2:
            return
        button = new_line[0]
        if new_line[1] == PUSH_KEY:
            controllerInput.inAction = True
            if button == LEFT_BUTTON_KEY:
                controllerInput.left = True
                controllerInput.leftButtonState = True
            if button == RIGHT_BUTTON_KEY:
                controllerInput.right = True
                controllerInput.rightButtonState = True
            if button == SHOOT_BUTTON_KEY:
                controllerInput.shoot = True
                controllerInput.shootButtonState = True
        elif new_line[1] == RELEASE_KEY:
            controllerInput.inAction = False 
            if button == LEFT_BUTTON_KEY:
                controllerInput.left = False
                controllerInput.leftButtonState = False
            if button == RIGHT_BUTTON_KEY:
                controllerInput.right = False
                controllerInput.rightButtonState = False
            if button == SHOOT_BUTTON_KEY:
                controllerInput.left = False
                controllerInput.leftButtonState = False
                
        if self.connected == False:
            logger.write_in_log("WARNING", __name__, "read", "All buttons are released")
            controllerInput.left = False
            controllerInput.leftButtonState = False
            controllerInput.right = False
            controllerInput.rightButtonState = False
            controllerInput.left = False
            controllerInput.leftButtonState = False