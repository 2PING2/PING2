from pingpy import *
from pingpy.config.config import BAUD_RATE, TIMEOUT, ports
from pingpy.input import Input
from pingpy.output import Output
from pingpy.debug import logger

class Ping:
    def __init__(self):
        self.input = Input()
        self.output = Output()
        self.esp32 = serialHard.ESP32Serial(ports["ESP32"], BAUD_RATE, TIMEOUT)
        self.UICorner = serialHard.UICornerSerial(ports["UICorner"], BAUD_RATE, TIMEOUT)
        logger.write_in_log("INFO", __name__, "__init__")
        
    def setup(self):
        self.esp32.setup()
        self.UICorner.setup()
        logger.write_in_log("INFO", __name__, "setup")