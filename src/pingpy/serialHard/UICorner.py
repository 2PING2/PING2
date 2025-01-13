from .serialCom import SerialCom
from pingpy.debug import logger

class UICornerSerial(SerialCom):
    def __init__(self, port, baud_rate, timeout):
        super().__init__(port, baud_rate, timeout)
        logger.write_in_log("INFO", "UICornerSerial", "__init__")