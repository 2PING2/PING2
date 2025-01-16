from .gameMode import GameMode
from ..output.output import Output
import time
from pingpy.debug import logger
from pingpy.config.config import PURPLE


class WaitingRoom(GameMode):
    """
    Game mode of Red Light Green Light. 1 2 3 soleil in French.
    """
    def __init__(self):
        logger.write_in_log("INFO", __name__, "__init__")
        
    def setup(self, output):
        for i in range(4):
            output.player[i].playerLedStrip.area = [-20, 20]
            output.player[i].playerLedStrip.color = PURPLE
        logger.write_in_log("INFO", __name__, "setup")
    
    def compute(self, input, output):
        pass
    

    def stop(self, input, output):
        for i in range(4):
            output.player[i].playerLedStrip.area = [-200, 200]
            output.player[i].playerLedStrip.color = (0, 0, 0)