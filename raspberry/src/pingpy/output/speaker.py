from pingpy.debug import logger

class SpeakerOutput:
    def __init__(self):
        self.audioPiste = None
        logger.write_in_log("INFO", __name__, "__init__")