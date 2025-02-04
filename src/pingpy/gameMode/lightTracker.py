from abc import ABC, abstractmethod
from pingpy.input.input import Input
from pingpy.output.output import Output, PlayerOutput
from datetime import datetime
from gameMode import GameMode
from config.config import YELLOW, PATH_AUDIO_LIGHT_TRACKER_INTRO
import time
import pingpy.debug.logFile
import random

#Creation du logfile


class LightTracker(GameMode):
    """
    Game mode of Light Tracker. Traque-lumière in french.
    """
    def __init__(self):
        self.areaLed=0
        self.ledCenter=0
        self.actualRound=0
        self.areaLength=20 ### ceci va varier selon la dificulté du jeu
        self.count_move=0
        self.color = YELLOW
        self.descriptionAudioPath = PATH_AUDIO_LIGHT_TRACKER_INTRO
    

    def setup(self, Input, Output):
        """
        Setup the game mode.
        """
        for playerInput in Input.ListPlayerInput:
            Output.ListPlayerOutput[playerInput.idPlayer].LinearActuatorOutput.move_to = 0
        self.ledCenter = random.uniform(Output.ListPlayerOutput[0].playerLedStrip.rightLimit, Output.ListPlayerOutput[0].playerLedStrip.leftLimit)
        self.areaLed = [self.ledCenter - self.areaLength/2, self.ledCenter + self.areaLength/2]
        Output.ListPlayerOutput[0].PlayerLedStrip.onPlayer(YELLOW, self.areaLed)
        self.actualRound += 1
        pass
    
    def run(self, Input, Output):
        """
        Run the game mode.
        """
        if self.actualRound == 0:
            self.setup(Input, Output)
        for playerInput in Input.ListPlayerInput:
            if self.check_victory(playerInput, Output):
                self.stop(Input, Output)
                return
            if not self.check_end_move(Input):
                self.move(playerInput, Output.ListPlayerOutput[playerInput.idPlayer])
            else:
                winnerId = self.check_distance(Input)
                for winner in winnerId:
                    self.modif_score(Input.ListPlayerInput[winner])
        self.new_round(Input, Output)

    def stop(self, Input, Output):
        for playerInput in Input.ListPlayerInput:
            playerInput.GameController3ButtonInput.countMove = 0
            Output.ListPlayerOutput[playerInput.idPlayer].LinearActuatorOutput.move_to = 0
            Output.ListPlayerOutput[playerInput.idPlayer].PlayerLedStrip.clear()

    def check_distance(self,Input):
        winnerId = []
        distance = float('inf')
        for playerInput in Input.ListPlayerInput:
            new_distance = abs(playerInput.linearActuator.currentPose - self.ledCenter)
            if new_distance <= distance:
                distance = new_distance
                winnerId = winnerId.append(playerInput.idPlayer)
        return winnerId 
        pass 

    def check_move(self, PlayerInput):
        return (
            PlayerInput.GameController3ButtonInput.countMove == 0 and
            (PlayerInput.GameController3ButtonInput.left or PlayerInput.GameController3ButtonInput.right)
        )

    def move(self, PlayerInput, PlayerOutput):
        if self.check_move(PlayerInput):
            PlayerInput.GameController3ButtonInput.countMove += 1
            if PlayerInput.GameController3ButtonInput.left:
                PlayerOutput.linearActuator.moveToLeft = True
                PlayerOutput.linearActuator.moveToRight = False
            elif PlayerInput.GameController3ButtonInput.right:
                PlayerOutput.linearActuator.moveToLeft = False
                PlayerOutput.linearActuator.moveToRight = True
        
    def check_end_move(self, Input):
        return all(not self.check_move(playerInput) for playerInput in Input.ListPlayerInput)

    def new_round(self,Input,Output):
        Output.PlayerOutput.LinearActuatorOutput.move_to = self.ledCenter
        newLedCenter = random.uniform(Input.PlayerInput.playerLedStrip.rightLimit, Input.PlayerInput.playerLedStrip.leftLimit)
        if newLedCenter == self.ledCenter:
            newPositionLed = random.uniform(Input.PlayerInput.playerLedStrip.rightLimit, Input.PlayerInput.playerLedStrip.leftLimit)
        self.ledCenter = newPositionLed
        self.areaLed = [self.ledCenter - self.areaLength/2, self.ledCenter + self.areaLength/2]
        self.actualRound += 1 
    
        pass

    def modif_score(self,PlayerInput):
        PlayerInput.pointCounter += 1
        pass


    def check_victory(self,PlayerInput, Output):
        if PlayerInput.pointCounter == 5:
            Output.ListPlayerOutput[PlayerInput.idPlayer].PlayerLedStrip.onPlayer(YELLOW)
            return True
        return False
        pass