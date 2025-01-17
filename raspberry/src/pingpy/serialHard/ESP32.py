from .serialCom import SerialCom
from pingpy.debug import logger
  
class ESP32Serial(SerialCom):
    def __init__(self, port, baudrate, timeout):
        super().__init__(port, baudrate, timeout)
        logger.write_in_log("INFO", __name__, "__init__")
        
    def read(self, input_ptr):
        """Read the next data from the serial port."""
        # new = self.ser.readline().decode('utf-8', errors='ignore').strip()
        # if new:
        #     logger.write_in_log("INFO", __name__, "read_data", f"Data received from {self.port}: {new}")
        #     self.queue.append(new)
        self.read_data_task()
        new_line = self.consume_older_data()
        # process the data
        # if new_line is not None:
        #     logger.write_in_log("INFO", __name__, "read", f"Read {new_line}")