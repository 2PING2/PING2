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
        
        self.gameModeList = [WaitingRoom(), RedLightGreenLight()]
        self.currentGameMode = WaitingRoom()
        # self.currentGameMode = self.gameModeList[0]
        self.prevGameMode = None
        # self.player1LedStrip = PlayerLedStrip(ledStrip, PLAYER_LED_STRIP_OFFSETS[1])
        self.playerLedStrip = [PlayerLedStrip(ledStrip, PLAYER_LED_STRIP_OFFSETS[i+1]) for i in range(4)]
        
        for i in range(4):
            self.input.player[i].gameController = GameController3ButtonInput()
        
        self.playerController = [ControllerSerial(self.input.player[i].gameController, ports["Player"][i], BAUD_RATE, TIMEOUT) for i in range(4)]
        logger.write_in_log("INFO", __name__, "__init__")
        
    def setup(self):
        self.esp32.setup()
        self.UICorner.setup()
        for i in range(4):
            self.playerController[i].setup()
        ledStrip.setup()
        ledStrip.clear()
        logger.write_in_log("INFO", __name__, "setup")
        
    def select_game_mode(self):
        pass
    
    def run(self):
        self.esp32.read(self.input)
        self.UICorner.read(self.input)
        for i in range(4):
            self.playerController[i].read(self.input.player[i])
            
        self.runGameMode()
        self.refresh_output()

    def runGameMode(self):
        logger.write_in_log("INFO", __name__, "runGameMode", f"Current game mode : {self.currentGameMode}")
        if self.prevGameMode!=self.currentGameMode:
            if self.prevGameMode is not None:
                self.prevGameMode.stop()
            self.currentGameMode.setup(self.input, self.output)
            self.prevGameMode = self.currentGameMode
        self.currentGameMode.compute(self.input, self.output)
            
    def refresh_output(self):
        # pass
        # self.esp32.write(self.output)
        # self.UICorner.write(self.output)
        # refresh led trip and speaker
        self.refresh_player_led_strip()
        
    def refresh_player_led_strip(self):
        for i in range(4):
            self.playerLedStrip[i].set_mm(self.output.player[i].playerLedStrip.area, self.output.player[i].playerLedStrip.color)
        ledStrip.show()
        
        


    def check_new_devices(self):
        devices = enumerate_serial_devices()
        added = devices.difference(self.old_devices)
        removed = self.old_devices.difference(devices)
        if added:
            print('added: {}'.format(added))
        if removed:
            print('removed: {}'.format(removed))
        return devices