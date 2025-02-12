from .serialCom import SerialCom
from pingpy.debug import logger
import subprocess
from pingpy.hardware import ledStrip
from pingpy.config.config import SEP_KEY, MODE_KEY, INCREMENT_KEY, DECREMENT_KEY, RESET_KEY, PUSH_KEY, RELEASE_KEY, VOLUME_KEY, LIGHT_KEY, LEVEL_KEY, MAX_VOLUME, RESET_DELAY_AFTER_BUTTON_PRESS, SHORT_PRESS_DELAY, LONG_PRESS_DELAY, ASK_STATUS_SETTINGS, STATUS_LED_KEY, STATUS_LED_ON, STATUS_LED_OFF, MAX_BRIGHTNESS, MODE_PB_KEY, STATUS_LED_FADEOUT, STATUS_LED_BLINK
import time 
import os
import sys

class UICornerSerial(SerialCom):
    def __init__(self, port, baud_rate, timeout):
        super().__init__(port, baud_rate, timeout)
        logger.write_in_log("INFO", __name__, "__init__")
        self.lastResetPressedTime = None
        
        self.resetButtonState = None
        self.modeButtonState = None
        self.longPressFlag = False
        
    def setup(self, output_ptr):
        super().setup()        
        output_ptr.UICorner.askForStatusSettings = True
        output_ptr.UICorner.statusLed = True  
              
        
    def manageResetButton(self, input_ptr, output_ptr, playerLedStrip):
        if self.resetButtonState is not None and not self.lastResetPressedTime is None:
            if self.resetButtonState and time.time() - self.lastResetPressedTime > RESET_DELAY_AFTER_BUTTON_PRESS:
                if self.modeButtonState is True:
                    logger.write_in_log("INFO", __name__, "read", "restart main.py")
                    self.send_data(STATUS_LED_KEY + SEP_KEY + STATUS_LED_BLINK)
                    ledStrip.clear()
                    ledStrip.show()
                    subprocess.Popen([sys.executable, sys.argv[0]])
                    time.sleep(1)
                    exit(0)
                    
                else:
                    logger.write_in_log("INFO", __name__, "read", "SHUTDOWN Raspberry.py")                   
                    ledStrip.clear()
                    ledStrip.show()
                    self.send_data(STATUS_LED_KEY + SEP_KEY + STATUS_LED_FADEOUT)               
                    subprocess.run(["sudo", "halt"])
                
            elif self.resetButtonState and time.time() - self.lastResetPressedTime > LONG_PRESS_DELAY and not self.longPressFlag:
                logger.write_in_log("INFO", __name__, "read", "long press")
                input_ptr.UICorner.resetLongPress = True
                self.longPressFlag = True
        
    def read(self, input_ptr, output_ptr, playerLedStrip):
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

        self.manageResetButton(input_ptr, output_ptr, playerLedStrip)
        
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
        if new_line[0] == MODE_PB_KEY:
            if new_line[1] == PUSH_KEY:
                self.modeButtonState = True
            elif new_line[1] == RELEASE_KEY:
                self.modeButtonState = False
            
                
        # reset
        if new_line[0] == RESET_KEY:
            if new_line[1] == PUSH_KEY:
                self.lastResetPressedTime = time.time()
                self.resetButtonState = True
                input_ptr.UICorner.resetPush = True
            elif new_line[1] == RELEASE_KEY:
                input_ptr.UICorner.resetRelease = True   
                self.resetButtonState = False
                self.longPressFlag = False
                if time.time() - self.lastResetPressedTime < SHORT_PRESS_DELAY:
                    input_ptr.UICorner.resetShortPress = True
                    logger.write_in_log("INFO", __name__, "read", "short press")
                      
        # volume              
        if new_line[0] == VOLUME_KEY:
            input_ptr.UICorner.volume = int(new_line[1])/1023.0*MAX_VOLUME
            output_ptr.speaker.volume = input_ptr.UICorner.volume
            
        # light
        if new_line[0] == LIGHT_KEY:
            input_ptr.UICorner.light = int(new_line[1])/1023.0     
            for playerOutput in output_ptr.player:
                playerOutput.playerLedStrip.brightness = input_ptr.UICorner.light     
    
        # level
        if new_line[0] == LEVEL_KEY:
            input_ptr.UICorner.level = int(new_line[1])/1023.0
            
    def write(self, output_ptr, input_ptr):
        """Write the next data to the serial port."""
        if output_ptr.UICorner.askForStatusSettings:
            output_ptr.UICorner.askForStatusSettings = None
            self.send_data(ASK_STATUS_SETTINGS + SEP_KEY)
        if not output_ptr.UICorner.statusLed is None:
            if output_ptr.UICorner.statusLed == True:
                self.send_data(STATUS_LED_KEY + SEP_KEY + STATUS_LED_ON)
            if output_ptr.UICorner.statusLed == False:
                self.send_data(STATUS_LED_KEY + SEP_KEY + STATUS_LED_OFF)
            output_ptr.UICorner.statusLed = None