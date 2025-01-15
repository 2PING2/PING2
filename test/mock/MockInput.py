class MockPlayerInput:
    def __init__(self, idPlayer):
        self.idPlayer = idPlayer
        self.GameController = MockGameController()
        self.LinearActuatorInput = MockLinearActuatorInput()


class MockGameController:
    def __init__(self):
        self.newAction = False


class MockLinearActuatorInput:
    def __init__(self):
        self.leftLimit = 0
        self.rightLimit = 100
        self.currentPose = 0


class MockInput:
    def __init__(self, num_players):
        self.ListPlayerInput = [MockPlayerInput(i) for i in range(num_players)]

