from abc import ABC, abstractmethod

"""
Mother class to all inputs 
Inputs : PlayerInput (linearActuatorInput, BeamSwitch...), GameController3button
"""

class Input:
    def __init__(self):
        self.ListPlayerInput = []
        pass
