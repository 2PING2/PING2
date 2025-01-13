from ..hardware.ledStrip import PlayerLedStrip
from linearActuator import LinearActuatorOutput


class PlayerOutput:
    def __init__(self, PlayerLedStrip, LinearActuatorOutput):
        self.PlayerLedStrip=PlayerLedStrip
        self.LinearActuatorOutput=LinearActuatorOutput
    pass
