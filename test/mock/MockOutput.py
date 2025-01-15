class MockPlayerLedStrip:
    def __init__(self):
        self.color = None

    def color(self, color):
        self.color = color


class MockLinearActuatorOutput:
    def __init__(self):
        self.move_to_right = False
        self.move_to_leftLimit = False
        self.move_to = None


class MockPlayerOutput:
    def __init__(self):
        self.PlayerLedStrip = MockPlayerLedStrip()
        self.LinearActuatorOutput = MockLinearActuatorOutput()
class MockLedStrip:
    def __init__(self):
        self.color = None
        
class MockOutput:
    def __init__(self, num_players):
        self.ListPlayerOutput = [MockPlayerOutput() for _ in range(num_players)]
        self.ledStrip = MockLedStrip()