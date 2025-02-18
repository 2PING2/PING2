from pingpy.debug import logger
from pingpy.input.player import PlayerInput
from pingpy.config.config import BAUD_RATE, TIMEOUT, ports, AUTO_PIN
from pingpy.input.UICorner import UICornerInput

"""
Mother class to all inputs 
Inputs : PlayerInput (linearActuatorInput, BeamSwitch...), GameController3button
"""
class Input:
    def __init__(self):
        self.player = [PlayerInput(ports["Player"][i], BAUD_RATE, TIMEOUT, AUTO_PIN["switch"][i], AUTO_PIN["led"][i]) for i in range(4)]
        self.UICorner = UICornerInput()
        logger.write_in_log("INFO", __name__, "__init__")