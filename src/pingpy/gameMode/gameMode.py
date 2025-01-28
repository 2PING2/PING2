from abc import ABC, abstractmethod
from pingpy.input.input import Input
from pingpy.output.output import Output
from datetime import datetime
import time
import random

class GameMode(ABC):
    """
    Mother class to all game mode that Should be inherited.
    Every game mode describe the behavior of outputs (LEDs, sound, and player moves) 
    based on the inputs (game controller, ball pose, and player pose). additional, it take UI panel as settings.
    """
    def __init__(self):
        super().__init__()
        self.inGame = False
    
    @abstractmethod
    def setup(self, output):
        pass
    
    @abstractmethod
    def compute(self, input, output):
        pass

    @abstractmethod
    def stop(self,Input): 
        pass

    pass