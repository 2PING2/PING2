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
        # self.currentGameMode = self.gameModeList[0]
        self.prevGameMode = None
        # self.player1LedStrip = PlayerLedStrip(ledStrip, PLAYER_LED_STRIP_OFFSETS[1])
        self.playerLedStrip = [PlayerLedStrip(ledStrip, PLAYER_LED_STRIP_OFFSETS[i+1]) for i in range(4)]
        
        for i in range(4):
            self.input.player[i].gameController = GameController3ButtonInput()
        
        self.playerController = [ControllerSerial(self.input.player[i].gameController, ports["Player"][i], BAUD_RATE, TIMEOUT) for i in range(4)]
        logger.write_in_log("INFO", __name__, "__init__")
        
    def setup(self):
        self.esp32.setup(self.output)
        self.UICorner.setup()
        for i in range(4):
            self.playerController[i].setup()
        ledStrip.setup()
        ledStrip.clear()
        # self.esp32.send_data("P{1}/C")
        logger.write_in_log("INFO", __name__, "setup")
        
        
        
    
    def run(self):
        self.esp32.read(self.input)
        self.UICorner.read(self.input)
        for i in range(4):
            try:
                self.playerController[i].read(self.input.player[i].gameController)
            except Exception as e:
                logger.write_in_log("ERROR", __name__, "run", f"Error in playerController[{i}].read: {e}")
        self.runGameMode()
        self.refresh_output()

    def runGameMode(self):
        if self.prevGameMode!=self.currentGameMode:
            if self.prevGameMode is not None:
                self.prevGameMode.stop()
            self.currentGameMode.setup(self.input, self.output)
            self.prevGameMode = self.currentGameMode
        self.waitingRoom.currentGameMode = self.currentGameMode
        self.currentGameMode.compute(self.input, self.output)
        self.currentGameMode = self.waitingRoom.currentGameMode
            
    def refresh_output(self):
        # pass
        self.esp32.write(self.output, self.input)
        # self.UICorner.write(self.output)
        self.output.speaker.play()
        try :
            self.refresh_player_led_strip()
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "refresh_output", f"Error in refresh_player_led_strip: {e}")
        
    def refresh_player_led_strip(self):
        for i in range(4):
            self.playerLedStrip[i].set_mm(self.output.player[i].playerLedStrip.area, self.output.player[i].playerLedStrip.color)
            
        ledStrip.show()