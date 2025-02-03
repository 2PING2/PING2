from pingpy.debug import logger

class UICornerInput:
    def __init__(self):
        self.light=None
        self.modeInc=None
        self.modeDec=None
        self.modePush=None
        self.modeRelease=None
        self.volume=None
        self.level=None
        self.resetPush=None
        self.resetRelease=None
        self.resetShortPress=None
        self.resetLongPress=None
        logger.write_in_log("INFO", __name__, "__init__")