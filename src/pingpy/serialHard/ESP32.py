from .serialCom import SerialCom
from ..input.input import Input 
from pingpy.debug import logger
  
class ESP32Serial(SerialCom):
    def __init__(self, port, baudrate, timeout):
        super().__init__(port, baudrate, timeout)
        logger.write_in_log("INFO", "ESP32Serial", "__init__")