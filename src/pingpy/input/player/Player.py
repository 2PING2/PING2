class PlayerInput:
    def __init__(self, BeamSwitch, LinearActuatorInput, GameController, idPlayer, pointCounter, lifePointCounter):
        self.BeamSwitch=BeamSwitch
        self.LinearActuatorInput=LinearActuatorInput
        self.GameController=GameController
        self.idPlayer=idPlayer
        self.pointCounter=pointCounter
        self.lifePointCounter=lifePointCounter
    pass