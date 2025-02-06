"""
This file is part of the PING² project.
Copyright (c) 2024 PING² Team

This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International (CC BY-NC-ND 4.0).
You may share this file as long as you credit the original author.

RESTRICTIONS:
- Commercial use is prohibited.
- No modifications or adaptations are allowed.
- See the full license at: https://creativecommons.org/licenses/by-nc-nd/4.0/

For inquiries, contact us at: projet.ping2@gmail.com
"""

try:
    import RPi.GPIO as GPIO  # Sur Raspberry Pi
except ImportError:
    from .gpioMock import GPIO  # Sur PC ou autre environnement
    
import threading
from pingpy.debug import logger



''' This class includes 1 switch and 1 LED. it will be used to select auto mode '''
class AutoSwitch:
    def __init__(self, AUTO_SWITCH_PIN, AUTO_LED_PIN):
        """Init states"""
        self.ledState = False
        self.mode = False
        self.buttonPushedFlag = False
        self.buttonState = False
        self.buttonReleasedFlag = False
        self.autoSwitchPin = AUTO_SWITCH_PIN
        self.autoLedPin = AUTO_LED_PIN
        logger.write_in_log("INFO", __name__, "__init__")
            
    def setup(self):
        """Config GPIO"""
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)

        # SWITCH
        GPIO.setup(self.autoSwitchPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

        # LED
        GPIO.setup(self.autoLedPin, GPIO.OUT)
        GPIO.output(self.autoLedPin, GPIO.LOW)
        
        logger.write_in_log("INFO", "autoSwitch", "setup", "Autoswitch and Autoled is initalized")
            
    def monitor_switch(self):
        """Read the state of the switch and update the LED"""
        gpioStatus = GPIO.input(self.autoSwitchPin)
        if gpioStatus == GPIO.LOW: # switch pushed
            if self.buttonState != gpioStatus: # changed
                print("button pushed")
                self.buttonState = gpioStatus
                self.buttonPushedFlag = True                     
        # Put the LED on or off        
        if self.ledState != self.mode:
            self.ledState = self.mode
            GPIO.output(self.autoLedPin, GPIO.HIGH if self.ledState else GPIO.LOW)
            
        return self.buttonPushedFlag

                