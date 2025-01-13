
"""
Mother class to all inputs 
Inputs : PlayerInput (linearActuatorInput, BeamSwitch...), GameController3button
"""

from pingpy.debug import logger

class Input:
    def __init__(self):
        logger.write_in_log("INFO", "input", "__init__")
        pass
