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
import pyudev
import time
from serial.tools import list_ports  # pyserial

def enumerate_serial_devices():
    return set([item for item in list_ports.comports()])




# Configurer le contexte udev
context = pyudev.Context()
monitor = pyudev.Monitor.from_netlink(context)
monitor.filter_by(subsystem='tty')  # Filtrer les événements des ports série (ou USB si nécessaire)



class Ping:
    def __init__(self):
        # Quick and dirty timing loop 
        self.old_devices = enumerate_serial_devices()

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
        self.check_usb_event()
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
        removed = old_devices.difference(devices)
        if added:
            print('added: {}'.format(added))
        if removed:
            print('removed: {}'.format(removed))
        return devices


    def check_usb_event(self):
        self.old_devices = self.check_new_devices()
        return
        for device in iter(monitor.poll, None):
            device_path = os.path.realpath(device.device_node)  # Résoudre les symlinks vers les chemins réels
            if device_path is None:
                continue
            # get the symlink of the device
            
            logger.write_in_log("INFO", __name__, "check_usb_event", f"Device {device.device_node} {device.action}")
           
            for i in range(4):
                playerControllerSerial = self.input.player[i].usb
                try:
                    logger.write_in_log("INFO", __name__, "check_usb_event", f"Checking {playerControllerSerial.port}->{os.readlink(playerControllerSerial.port)} =?= {device_path}")
                    if os.readlink(playerControllerSerial.port) not in device_path:
                        continue
                except FileNotFoundError:
                    logger.write_in_log("WARNING", __name__, "check_usb_event", f"Port {playerControllerSerial.port} not found")
                    continue
                
                logger.write_in_log("INFO", __name__, "check_usb_event", f"Device {device.device_node} {device.action} with path {device_path} on {playerControllerSerial.port}")
                if device.action == 'add':
                    logger.write_in_log("INFO", __name__, "check_usb_event", f"Device connected to {playerControllerSerial.port}")
                    playerControllerSerial.setup()
                elif device.action == 'remove':
                    playerControllerSerial.stop_reading()
                    logger.write_in_log("INFO", __name__, "check_usb_event", f"Device disconnected from {playerControllerSerial.port}")
                else:
                    logger.write_in_log("INFO", __name__, "check_usb_event", f"event {device.action} on {playerControllerSerial.port}")
            break