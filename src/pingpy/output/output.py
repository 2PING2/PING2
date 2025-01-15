from pingpy.debug import logger
from pingpy.output.player import PlayerOutput
from pingpy.output.speaker import SpeakerOutput


class Output:
    def __init__(self):
        self.ListPlayerOutput = [PlayerOutput() for _ in range (4)]
        self.speaker = SpeakerOutput()
        logger.write_in_log("INFO", __name__, "__init__")
    """ def __init__(self):
        self.ListPlayerOutput = []
        self.ledStrip = LedStrip("LED_STRIP_PIN", "NUMBER_OF_LEDS", "FREQUENCY", "DMA_CHANNEL", "BRIGHTNESS")
        pass
 """


