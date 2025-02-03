from pingpy.debug import logger

class UICornerOutput:
    def __init__(self):
        self.askForStatusSettings = False
        self.statusLed = None
        logger.write_in_log("INFO", __name__, "__init__")