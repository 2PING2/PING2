# mock_gpio.py
class GPIO:
    BCM = "BCM"
    BOARD = "BOARD"
    OUT = "OUT"
    IN = "IN"
    HIGH = True
    LOW = False

    @staticmethod
    def setmode(mode):
        print(f"Setting GPIO mode to {mode}")

    @staticmethod
    def setup(pin, mode):
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
