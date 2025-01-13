from pingpy.debug import logger
from pingpy.output.playerLedStrip import PlayerLedStripOutput
from pingpy.output.linearActuator import LinearActuatorOutput
from pingpy.output.bumper import BumperOutput

class PlayerOutput:
    def __init__(self):
        self.playerLedStrip=PlayerLedStripOutput()
        self.linearActuator=LinearActuatorOutput()
        self.bumper=BumperOutput()
        logger.write_in_log("INFO", __name__, "__init__")
