from pingpy.debug import logger
from .Player import PlayerInput
from pingpy.config.config import BAUD_RATE, TIMEOUT, ports
from pingpy.input.UICorner import UICornerInput

"""
Mother class to all inputs 
Inputs : PlayerInput (linearActuatorInput, BeamSwitch...), GameController3button
"""
class Input:
    def __init__(self):
        self.ListPlayerInput = [PlayerInput(ports["Player"+str(i)], BAUD_RATE, TIMEOUT) for i in range(1, 5)]
        self.UICorner = UICornerInput()
        logger.write_in_log("INFO", __name__, "__init__")