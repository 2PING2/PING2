from .serialCom import SerialCom
from pingpy.debug import logger
from pingpy.config.config import PLAYER_KEY, PARAM_BEGIN_SEP, PARAM_END_SEP, KEY_SEP, MOVE_TO_LEFT_LIMIT_KEY, MOVE_TO_RIGHT_LIMIT_KEY, STOP_KEY
  
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
        
    def write(self, output_ptr):
        """Write the next data to the serial port."""
        for i in range(len(output_ptr.player)):
            playerOutput = output_ptr.player[i]
            if playerOutput.linearActuator.moveToRight:
                logger.write_in_log("INFO", __name__, "write", f"Player {i} move to right.")
                playerOutput.linearActuator.moveToRight = None
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i) + PARAM_END_SEP + KEY_SEP + MOVE_TO_RIGHT_LIMIT_KEY)
            if playerOutput.linearActuator.moveToLeft:
                playerOutput.linearActuator.moveToLeft = None
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i) + PARAM_END_SEP + KEY_SEP + MOVE_TO_LEFT_LIMIT_KEY)
            if playerOutput.linearActuator.stop:
                playerOutput.linearActuator.stop = None
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i) + PARAM_END_SEP + KEY_SEP + STOP_KEY)