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

import RPi.GPIO as GPIO
import threading
from logFile import LogFile
log = LogFile()

''' This class includes 1 switch and 1 LED. it will be used to select auto mode '''
class AutoSwitch:
    def __init__(self, AUTO_SWITCH_PIN, AUTO_LED_PIN):
        """Init states"""
        self.ledState = False
        self.autoMode = False
        self.autoSwitchPin = AUTO_SWITCH_PIN
        self.autoLedPin = AUTO_LED_PIN
        self.monitor_thread = None 

    def setup(self):
        """Config GPIO"""
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)

        # SWITCH
        GPIO.setup(self.autoSwitchPin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

        # LED
        GPIO.setup(self.autoLedPin, GPIO.OUT)
        GPIO.output(self.autoLedPin, GPIO.LOW)
        
        log.write_in_log("INFO", "autoSwitch", "setup", "Autoswitch and Autoled is initalized")
            
        # start monitor_switch in a thread
        self.monitor_thread = threading.Thread(target=self.monitor_switch(self.autoSwitchPin, self.autoLedPin), daemon=True)
        self.monitor_thread.start()

    def monitor_switch(self):
        """Read the state of the switch and update the LED"""
        buttonStates = False         
        if GPIO.input(self.autoSwitchPin) == GPIO.LOW: # switch pushed
            if not buttonStates: # if the button is not already pushed
                self.ledState = not self.ledState
                self.autoMode = self.ledState

                # Put the LED on or off
                GPIO.output(self.autoLedPin, GPIO.HIGH if self.ledState else GPIO.LOW)

                buttonStates = True # record the button state
        else:
            buttonStates = False # switch released
                