from ..hardware.ledStrip import LedStrip, PlayerLedStrip


class Output:
    def __init__(self):
        self.ListPlayerOutput = []
        self.ledStrip = LedStrip("LED_STRIP_PIN", "NUMBER_OF_LEDS", "FREQUENCY", "DMA_CHANNEL", "BRIGHTNESS")
        pass


