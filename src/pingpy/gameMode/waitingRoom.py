from .gameMode import GameMode
from ..output.output import Output
import time
from pingpy.debug import logger
from pingpy.config.config import PURPLE
import time

class WaitingRoom(GameMode):
    """
    Game mode of Red Light Green Light. 1 2 3 soleil in French.
    """
    def __init__(self):
        logger.write_in_log("INFO", __name__, "__init__")
        self.currentLed_brightness = 0.0
        self.brightness_blink_rate = 0.5
        self.last_time = time.time()
        
    def setup(self, output):
        for i in range(4):
            output.player[i].playerLedStrip.area = [-200, 200]
            output.player[i].playerLedStrip.color = tuple(round(x * self.currentLed_brightness) for x in PURPLE)
        logger.write_in_log("INFO", __name__, "setup")
    
    def compute(self, input, output):
        for i in range(4):
            output.player[i].playerLedStrip.color = tuple(round(x * self.currentLed_brightness) for x in PURPLE)
            if input.player[i].gameController.inAction:
                output.player[i].playerLedStrip.color = (50, 50, 50)
                
        t = time.time()
        dt = t - self.last_time
        self.last_time = t
        
        self.currentLed_brightness += self.brightness_blink_rate * dt
        if self.currentLed_brightness > 1.0:
            self.currentLed_brightness = 1.0
            self.brightness_blink_rate *= -1
        elif self.currentLed_brightness < 0.0:
            self.currentLed_brightness = 0.0
            self.brightness_blink_rate *= -1
        
    

    def stop(self, input, output):
        for i in range(4):
            output.player[i].playerLedStrip.area = [-200, 200]
            output.player[i].playerLedStrip.color = (0, 0, 0)