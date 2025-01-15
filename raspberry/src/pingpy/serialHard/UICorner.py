from .serialCom import SerialCom
from pingpy.debug import logger

class UICornerSerial(SerialCom):
    def __init__(self, port, baud_rate, timeout):
        super().__init__(port, baud_rate, timeout)
        logger.write_in_log("INFO", __name__, "__init__")
        
    def read(self, input_ptr):
        """Read the next data from the serial port."""
        new_line = self.consume_older_data()
        # process the data
        if new_line is not None:
            logger.write_in_log("INFO", __name__, "read", f"Read {new_line}")