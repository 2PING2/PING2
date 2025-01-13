from pingpy.debug import logger


class BeamSwitchInput():
    def __init__(self):
        self.state = None
        logger.write_in_log("INFO", __name__, "__init__")

    
        