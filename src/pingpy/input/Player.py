from pingpy.debug import logger
from pingpy.input.beamSwitch import BeamSwitchInput
from pingpy.input.linearActuator import LinearActuatorInput
from pingpy.input.gameController import GameControllerInput
from pingpy.config.config import CONTROLLER_TYPE_3BUTTONS, CONTROLLER_TYPE_1BUTTON_1JOYSTICK
from pingpy.serialHard.controller import ControllerSerial

class PlayerInput:
    def __init__(self, usbPort, usbBaudRate, usbTimeout):
        self.beamSwitch = BeamSwitchInput()
        self.linearActuatorInput = LinearActuatorInput()
        self.gameController = GameControllerInput()
        self.usb = ControllerSerial(self.gameController, usbPort, usbBaudRate, usbTimeout)
        logger.write_in_log("INFO", __name__, "__init__")
        
        
