from .serialCom import SerialCom
from pingpy.debug import logger
import subprocess
from pingpy.config.config import SEP_KEY, MODE_KEY, INCREMENT_KEY, DECREMENT_KEY, RESET_KEY, PUSH_KEY, RELEASE_KEY, VOLUME_KEY, LIGHT_KEY, LEVEL_KEY, MAX_VOLUME

class UICornerSerial(SerialCom):
    def __init__(self, port, baud_rate, timeout):
        super().__init__(port, baud_rate, timeout)
        logger.write_in_log("INFO", __name__, "__init__")
        
    def read(self, input_ptr):
        """Read the next data from the serial port."""
        try:
            self.read_data_task()
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read", f"Error in read_data_task: {e}")
            return
        try:
            new_line = self.consume_older_data()
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read", f"Error in consume_older_data: {e}")
            return
        if new_line is None:
            return
        try:
            new_line = new_line.split(SEP_KEY)
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read", f"Error in split: {e}")
            return
        if len(new_line) < 2:
            return
        
        # mode
        if new_line[0] == MODE_KEY:
            if new_line[1] == INCREMENT_KEY:
                input_ptr.UICorner.modeInc = True
            elif new_line[1] == DECREMENT_KEY:
                input_ptr.UICorner.modeDec = True
                
        # reset
        if new_line[0] == RESET_KEY:
            if new_line[1] == PUSH_KEY:
                input_ptr.UICorner.resetPush = True
            elif new_line[1] == RELEASE_KEY:
                input_ptr.UICorner.resetRelease = True    
                
        if new_line[0] == VOLUME_KEY:
            input_ptr.UICorner.volume = int(int(new_line[1])/1023.0*MAX_VOLUME)
            subprocess.run(["amixer", "set", "Master", f"{input_ptr.UICorner.volume}%"]) 
                
            