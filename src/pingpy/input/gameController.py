class GameControllerInput(ABC):
    @abstractmethod
    def __init__(self, newAction, newActionleft, newActionright, newActionShoot):
        pass
