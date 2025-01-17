from pingpy.debug import logger
from pingpy.output.player import PlayerOutput
from pingpy.output.speaker import SpeakerOutput


class Output:
    def __init__(self):
        self.player = [PlayerOutput() for _ in range (4)]
        self.speaker = SpeakerOutput()
        logger.write_in_log("INFO", __name__, "__init__")
 

