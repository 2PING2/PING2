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
        # self.esp32.send_data("P{1}/C")
        logger.write_in_log("INFO", __name__, "setup")
        
    def select_game_mode(self):
        pass
    
    def run(self):
        self.esp32.read(self.input)
        self.UICorner.read(self.input)
        for i in range(4):
            try:
                self.playerController[i].read(self.input.player[i])
            except Exception as e:
                logger.write_in_log("ERROR", __name__, "run", f"Error in playerController[{i}].read: {e}")
        logger.write_in_log("INFO", __name__, "run", f"playerController[0].ser: {self.playerController[0].ser}")

        self.runGameMode()
        self.refresh_output()

    def runGameMode(self):
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
        # self.esp32.send_data("P{1}/C")
        try :
            self.refresh_player_led_strip()
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "refresh_output", f"Error in refresh_player_led_strip: {e}")
        try:
            for i in range(4):
                if self.output.player[i].linearActuator.moveToLeft is not None:
                    if self.output.player[i].linearActuator.moveToLeft:
                        self.esp32.send_data(f"P{{{i+1}}}/MTLL")
                    else:
                        self.esp32.send_data(f"P{{{i+1}}}/S")
                    self.output.player[i].linearActuator.moveToLeft = None
                if self.output.player[i].linearActuator.moveToRight is not None:
                    if self.output.player[i].linearActuator.moveToRight:
                        self.esp32.send_data(f"P{{{i+1}}}/MTRL")
                    else:
                        self.esp32.send_data(f"P{{{i+1}}}/S")
                    self.output.player[i].linearActuator.moveToRight = None
                if self.output.player[i].bumper.shoot is not None:
                    if self.output.player[i].bumper.shoot:
                        self.esp32.send_data(f"P{{{i+1}}}/S")
                    self.output.player[i].bumper.shoot = None
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "refresh_output", f"Error in refresh_output: {e}")
        
    def refresh_player_led_strip(self):
        for i in range(4):
            self.playerLedStrip[i].set_mm(self.output.player[i].playerLedStrip.area, self.output.player[i].playerLedStrip.color)
            
        ledStrip.show()