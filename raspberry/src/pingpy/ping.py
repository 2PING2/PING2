from pingpy import *
from pingpy.config.config import BAUD_RATE, TIMEOUT, ports, PLAYER_LED_STRIP_OFFSETS, UI_CORNER_BAUD_RATE, GREEN, ORANGE, YELLOW, RED, BLUE
from pingpy.input import Input
from pingpy.input.gameController3Button import GameController3ButtonInput
from pingpy.output import Output
from pingpy.debug import logger
from pingpy.hardware import ledStrip
from pingpy.hardware.ledStrip import PlayerLedStrip
from pingpy.serialHard.controller import ControllerSerial
from pingpy.gameMode import *


class Ping:
    def __init__(self):
        self.input = Input()
        self.output = Output()
        self.esp32 = serialHard.ESP32Serial(ports["ESP32"], BAUD_RATE, TIMEOUT)
        self.UICorner = serialHard.UICornerSerial(ports["UICorner"], UI_CORNER_BAUD_RATE, TIMEOUT)
        self.gameModeList = [RedLightGreenLight()]
        self.currentGameMode = None
        self.waitingRoom = WaitingRoom(self.gameModeList, self.currentGameMode)
        self.currentGameMode = self.waitingRoom
        self.prevGameMode = None
        self.playerLedStrip = [PlayerLedStrip(ledStrip, PLAYER_LED_STRIP_OFFSETS[i+1]) for i in range(4)]
        
        for i in range(4):
            self.input.player[i].gameController = GameController3ButtonInput()
        
        self.playerController = [ControllerSerial(self.input.player[i].gameController, ports["Player"][i], BAUD_RATE, TIMEOUT) for i in range(4)]
        logger.write_in_log("INFO", __name__, "__init__")
        
    def setup(self):
        self.esp32.setup(self.output)
        self.UICorner.setup(self.output)
        for i in range(4):
            self.playerController[i].setup()
        ledStrip.setup()
        ledStrip.clear()
        logger.write_in_log("INFO", __name__, "setup")
       
    
    def run(self):
        self.esp32.read(self.input)
        self.UICorner.read(self.input, self.output)
        for i in range(4):
            try:
                self.playerController[i].read(self.input.player[i].gameController, self.output.player[i])
            except Exception as e:
                logger.write_in_log("ERROR", __name__, "run", f"Error in playerController[{i}].read: {e}")
        self.runGameMode()
        self.refresh_output()

    def runGameMode(self):
        if self.prevGameMode!=self.currentGameMode:
            if self.prevGameMode is not None:
                self.prevGameMode.stop(self.output)
            self.currentGameMode.setup(self.input, self.output)
            self.prevGameMode = self.currentGameMode
        self.waitingRoom.currentGameMode = self.currentGameMode
        self.currentGameMode.compute(self.input, self.output)
        self.currentGameMode = self.waitingRoom.currentGameMode
        if self.input.UICorner.resetLongPress:
            logger.write_in_log("INFO", __name__, "runGameMode", "return to waiting room")
            self.input.UICorner.resetLongPress = None
            self.input.UICorner.modeInc = None
            self.input.UICorner.modeDec = None
            self.currentGameMode = self.waitingRoom
            
            
    def refresh_output(self):
        self.esp32.write(self.output, self.input)
        self.UICorner.write(self.output, self.input)
        self.output.speaker.play()
        self.refresh_player_led_strip()        
        
    def refresh_player_led_strip(self):
        for i in range(4):
            if self.output.player[i].playerLedStrip.brightness is not None:
                self.playerLedStrip[i].set_brightness(self.output.player[i].playerLedStrip.brightness)
            if self.output.player[i].playerLedStrip.area is not None and self.output.player[i].playerLedStrip.color is not None:
                self.playerLedStrip[i].set_mm(self.output.player[i].playerLedStrip.area, self.output.player[i].playerLedStrip.color)

        ledStrip.show()