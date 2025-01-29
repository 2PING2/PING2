from .serialCom import SerialCom
from pingpy.debug import logger
import subprocess
from pingpy.config.config import SEP_KEY, MODE_KEY, INCREMENT_KEY, DECREMENT_KEY, RESET_KEY, PUSH_KEY, RELEASE_KEY, VOLUME_KEY, LIGHT_KEY, LEVEL_KEY, MAX_VOLUME, RESET_DELAY_AFTER_BUTTON_PRESS, SHORT_PRESS_DELAY, LONG_PRESS_DELAY
import time 
import os

class UICornerSerial(SerialCom):
    def __init__(self, port, baud_rate, timeout):
        super().__init__(port, baud_rate, timeout)
        logger.write_in_log("INFO", __name__, "__init__")
        self.lastResetPressedTime = None
        self.resetButtonState = None
        
    def manageResetButton(self, input_ptr):
        if self.resetButtonState is not None and not self.lastResetPressedTime is None:
            if self.resetButtonState and time.time() - self.lastResetPressedTime > RESET_DELAY_AFTER_BUTTON_PRESS:
                logger.write_in_log("INFO", __name__, "read", "restart main.py")
                os.system(f'sleep 0.1 && python3 /home/pi/Documents/PING2/raspberry/src/main.py')

            elif self.resetButtonState and time.time() - self.lastResetPressedTime > LONG_PRESS_DELAY:
                logger.write_in_log("INFO", __name__, "read", "long press")
                input_ptr.UICorner.resetLongPress = True
        
    def read(self, input_ptr, output_ptr):
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

        self.manageResetButton(input_ptr)
        
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
                self.lastResetPressedTime = time.time()
                self.resetButtonState = True
                input_ptr.UICorner.resetPush = True
            elif new_line[1] == RELEASE_KEY:
                input_ptr.UICorner.resetRelease = True   
                self.resetButtonState = False
                if time.time() - self.lastResetPressedTime < SHORT_PRESS_DELAY:
                    input_ptr.UICorner.resetShortPress = True
                                    
        if new_line[0] == VOLUME_KEY:
            input_ptr.UICorner.volume = int(new_line[1])/1023.0*MAX_VOLUME
            output_ptr.speaker.volume = input_ptr.UICorner.volume
                
            