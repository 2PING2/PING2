from abc import ABC, abstractmethod
from classes.input.input import Input
from classes.output.output import Output
from datetime import datetime
from gameMode import GameMode
from classes.config import YELLOW
import time
from classes.logFile import LogFile
import random

#Creation du logfile
log = LogFile()


class LightTracker(GameMode):
    """
    Game mode of Light Tracker. Traque-lumière in french.
    """
    def __init__(self, positionLed, distanceLed):
        self.positionLed=positionLed
        self.distanceLed=distanceLed
        self.roundNumber=5 #5 rounds
        self.actualRound=0
        self.outputData = Output([])
    
    def run(self,PlayerInput):
        pass

    def stop(self,PlayerInput):
        pass

    def check_distance(self,PlayerInput):
        distance = abs(PlayerInput.LinearActuatorInput.currentPose - self.positionLed)
        return distance
        pass 

    def move(self,PlayerInput):
        if PlayerInput.Manette.newAction:
            if PlayerInput.Manette.newActionLeft:
                PlayerInput.LinearActuatorOutput.move_to_left = True
                PlayerInput.LinearActuatorOutput.move_to_right = False
            elif PlayerInput.Manette.newActionRight:
                PlayerInput.LinearActuatorOutput.move_to_left = False
                PlayerInput.LinearActuatorOutput.move_to_right = True
        pass

    def check_move(self,PlayerInput,Input):
        for PlayerInput in Input.ListPlayerInput:
            if PlayerInput.Manette.newAction == True:
                return False
            else:
                return True
        pass

    def check_hold_on(self,PlayerInput):
        """This function checks if the player is holding down the controller button"""
        

        pass

    def new_round(self,PlayerInput):
        newPositionLed = random.uniform(PlayerInput.LinearActuatorInput.rightLimit, PlayerInput.LinearActuatorInput.leftLimit)
        if newPositionLed == self.positionLed:
            newPositionLed = random.uniform(PlayerInput.LinearActuatorInput.rightLimit, PlayerInput.LinearActuatorInput.leftLimit)
        else:
            self.positionLed = newPositionLed
            self.actualRound += 1
        pass

    def modif_score(self,PlayerInput):
        PlayerInput.pointCounter += 1
        pass


    def check_victory(self,PlayerInput):
        if PlayerInput.pointCounter == 5:
            self.outputData.ListPlayerOutput[PlayerInput.idPlayer].PlayerLedStrip.onPlayer(YELLOW)
            return True
        return False
        pass