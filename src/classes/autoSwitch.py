import RPi.GPIO as GPIO
import threading
from config import AUTO_SWITCH1_PIN, AUTO_SWITCH2_PIN, AUTO_SWITCH3_PIN, AUTO_SWITCH4_PIN, AUTO_LED1_PIN, AUTO_LED2_PIN, AUTO_LED3_PIN, AUTO_LED4_PIN
from logFile import LogFile
log = LogFile()

AUTO_SWITCHS_PIN = [AUTO_SWITCH1_PIN, AUTO_SWITCH2_PIN, AUTO_SWITCH3_PIN, AUTO_SWITCH4_PIN]
AUTO_LEDS_PIN = [AUTO_LED1_PIN, AUTO_LED2_PIN, AUTO_LED3_PIN, AUTO_LED4_PIN]

class AutoSwitch:
    def __init__(self):
        """Init states"""
        self.led_states = [False] * len(AUTO_LEDS_PIN)
        self.auto_modes = [False] * len(AUTO_SWITCHS_PIN)
        self.running = False
        self.monitor_thread = None 

    def setup(self):
        """Config GPIO"""
        GPIO.setmode(GPIO.BCM)
        GPIO.setwarnings(False)

        for AUTO_LED_PIN in AUTO_LEDS_PIN:
            GPIO.setup(AUTO_LED_PIN, GPIO.OUT)
            GPIO.output(AUTO_LED_PIN, GPIO.LOW)

        for AUTO_SWITCH_PIN in AUTO_SWITCHS_PIN:
            GPIO.setup(AUTO_SWITCH_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
            
        log.write_in_log("INFO", "AutoSwitch", "setup", "AutoSwitchs and AutoLeds initialized")  

    def start(self):
        """Start monitoring switches"""
        self.monitor_thread = threading.Thread(target=self.monitor_switchs, daemon=True)
        self.monitor_thread.start()

    def monitor_switchs(self):
        """Read the state of the switches and update the LEDs"""
        button_states = [False] * len(AUTO_SWITCHS_PIN)
        for i, button_pin in enumerate(AUTO_SWITCHS_PIN):
            if GPIO.input(button_pin) == GPIO.LOW: # switch pushed
                if not button_states[i]: # if the button is not already pushed
                    self.led_states[i] = not self.led_states[i]
                    self.auto_modes[i] = self.led_states[i]

                    # Put the LED on or off
                    GPIO.output(AUTO_LEDS_PIN[i], GPIO.HIGH if self.led_states[i] else GPIO.LOW)

                    button_states[i] = True # record the button state
            else:
                button_states[i] = False # switch released