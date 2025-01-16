from pingpy import *
from pingpy.config.config import BAUD_RATE, TIMEOUT, ports, PLAYER_LED_STRIP_OFFSETS, GREEN, ORANGE, YELLOW, RED, BLUE
from pingpy.input import Input
from pingpy.output import Output
from pingpy.debug import logger
from pingpy.hardware import ledStrip
from pingpy.hardware.ledStrip import PlayerLedStrip

from pingpy.gameMode import *

class Ping:
    def __init__(self):
        self.input = Input()
        self.output = Output()
        self.esp32 = serialHard.ESP32Serial(ports["ESP32"], BAUD_RATE, TIMEOUT)
        self.UICorner = serialHard.UICornerSerial(ports["UICorner"], BAUD_RATE, TIMEOUT)
        self.currentGameMode = WaitingRoom()
        # self.player1LedStrip = PlayerLedStrip(ledStrip, PLAYER_LED_STRIP_OFFSETS[1])
        self.playerLedStrip = [PlayerLedStrip(ledStrip, PLAYER_LED_STRIP_OFFSETS[i+1]) for i in range(4)]
        logger.write_in_log("INFO", __name__, "__init__")
        
    def setup(self):
        self.esp32.setup()
        self.UICorner.setup()
        ledStrip.setup()
        ledStrip.clear()
        self.playerLedStrip[0].onPlayer(YELLOW)
        self.playerLedStrip[1].onPlayer(GREEN)
        self.playerLedStrip[2].onPlayer(RED)
        self.playerLedStrip[3].onPlayer(BLUE)
        logger.write_in_log("INFO", __name__, "setup")
        
    def select_game_mode(self):
        pass
    
    def run(self):
        # self.esp32.read(self.input)
        # self.UiCorner.read(self.input)
        # self.currentGameMode.compute(self.input, self.output)
        # self.refresh_output()
        pass
    
    def refresh_output(self):
        pass
        # self.esp32.write(self.output)
        # self.UICorner.write(self.output)
        # refresh led trip and speaker
        
        
        
        