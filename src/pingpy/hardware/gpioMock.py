# mock_gpio.py
from pingpy.debug import logger


logger.write_in_log("INFO", "gpioMock", "__init__", "GPIO mock loaded")

class GPIO:
    BCM = "BCM"
    BOARD = "BOARD"
    OUT = "OUT"
    IN = "IN"
    PUD_UP = "PUD_UP"
    HIGH = True
    LOW = False

    @staticmethod
    def setmode(mode):
        print(f"Setting GPIO mode to {mode}")

    @staticmethod
    def setup(pin, mode, pull_up_down= None):
        print(f"Setting up pin {pin} as {mode}")

    @staticmethod
    def output(pin, state):
        print(f"Setting pin {pin} to {'HIGH' if state else 'LOW'}")

    @staticmethod
    def input(pin):
        print(f"Reading pin {pin}")
        return GPIO.LOW  # Simule un état bas

    @staticmethod
    def cleanup():
        print("Cleaning up GPIO")

    @staticmethod
    def setwarnings(flag):
        print(f"Set warnings to {'on' if flag else 'off'}")
