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
    def __init__(self, gameModeList, currentGameMode):
        logger.write_in_log("INFO", __name__, "__init__")
        self.gameModeList = gameModeList
        self.preselectedGameMode = None
        self.preselectedGameModeFlag = False
        self.color = PURPLE
        self.currentColor = self.color
        self.currentGameMode = currentGameMode
        self.currentLed_brightness = 0.0
        self.brightness_blink_rate = 0.5
        self.last_time = time.time()
        
    def setup(self, input, output):
        self.preselectedGameMode = None
        self.preselectedGameModeFlag = False
        self.currentColor = self.color
        output.speaker.stop = True
        for i in range(4):
            output.player[i].playerLedStrip.area = [-200, 200]
            output.player[i].playerLedStrip.color = tuple(round(x * self.currentLed_brightness) for x in self.currentColor)
        logger.write_in_log("INFO", __name__, "setup")
    
    def compute(self, input, output):
        t = time.time()
        dt = t - self.last_time
        self.last_time = t
        
        ####### Blinking effect
        self.currentLed_brightness += self.brightness_blink_rate * dt
        if self.currentLed_brightness > 1.0:
            self.currentLed_brightness = 1.0
            self.brightness_blink_rate *= -1
        elif self.currentLed_brightness < 0.0:
            self.currentLed_brightness = 0.0
            self.brightness_blink_rate *= -1
            
        for i in range(4):
            output.player[i].playerLedStrip.area = [-200, 200]
            output.player[i].playerLedStrip.color = tuple(round(x * self.currentLed_brightness) for x in self.currentColor)
        ####### end of blinking effect
            
        
        if input.UICorner.modeInc:
            if self.preselectedGameMode is None:
                self.preselectedGameMode = 0
            input.UICorner.resetShortPress = None
            self.preselectedGameMode = (self.preselectedGameMode + 1) % len(self.gameModeList)
            input.UICorner.modeInc = None
            self.preselectedGameModeFlag = False
            
        elif input.UICorner.modeDec:
            if self.preselectedGameMode is None:
                self.preselectedGameMode = 0
            input.UICorner.resetShortPress = None
            self.preselectedGameMode = (self.preselectedGameMode - 1) % len(self.gameModeList)
            input.UICorner.modeDec = None
            self.preselectedGameModeFlag = False
            
        if self.preselectedGameMode is not None:
            if self.preselectedGameModeFlag is False:
                self.preselectedGameModeFlag = True
                self.currentColor = self.gameModeList[self.preselectedGameMode].color
                output.speaker.audioPiste = self.gameModeList[self.preselectedGameMode].descriptionAudioPath
                output.speaker.stop = True
                logger.write_in_log("INFO", __name__, "preselectedGameMode: " + str(self.preselectedGameMode))
            
            if input.UICorner.resetShortPress:
                self.currentGameMode = self.gameModeList[self.preselectedGameMode]
                output.speaker.stop = True
                self.preselectedGameMode = None
                input.UICorner.resetShortPress = None
                self.preselectedGameModeFlag = False
        
    def stop(self, output_ptr):
        pass
        # for i in range(4):
        #     output.player[i].playerLedStrip.area = [-200, 200]
        #     output.player[i].playerLedStrip.color = (0, 0, 0)