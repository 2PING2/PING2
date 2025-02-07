from abc import ABC, abstractmethod
from ..input.input import Input
from ..output.output import Output
from .gameMode import GameMode
from pingpy.config.config import BLUE, YELLOW, PATH_AUDIO_LIGHT_TRACKER_INTRO
import time
from pingpy.debug import logger
from random import uniform



class LightTracker(GameMode):
    """
    Game mode of Light Tracker. Traque-lumière in french.
    """
    def __init__(self):
        self.areaLed=0
        self.ledCenter=0
        self.actualRound=0
        self.areaLength=20 ### ceci va varier selon la dificulté du jeu
        self.color = BLUE
        self.descriptionAudioPath = PATH_AUDIO_LIGHT_TRACKER_INTRO

        logger.write_in_log("INFO", __name__, "__init__", "Game mode initialized.")

    def setup(self, Input, Output):
        """
        Setup the game mode.
        """
        
        self.ledCenter = uniform(-200+self.areaLength/2 ,200 - self.areaLength/2)
        self.areaLed = [self.ledCenter - self.areaLength/2, self.ledCenter + self.areaLength/2]
        for i in range(4):
            try:
                playerOutput = Output.player[i]
                playerInput = Input.player[i]
                playerOutput.linearActuator.moveTo = 0
                playerOutput.playerLedStrip.area = self.areaLed
            except IndexError:
                logger.write_in_log("ERROR", __name__, "setup", f"No output found for player ID {Input.playerInput[i]}.")

        self.actualRound += 1
        pass
    
    def compute(self, Input, Output):
        """
        Run the game mode.
        """
        if self.actualRound == 0:
            self.setup(Input, Output)
        for i in range(4):
            if self.check_victory(Input.player[i], Output):
                self.stop(Input, Output)
                return
            if not self.check_end_move(Input):
                self.move(Input.player[i], Output.player[i])    
            else:
                winnerId = self.check_distance(Input)
                for winner in winnerId:
                    self.modif_score(Input.player[winner])
        self.new_round(Input, Output)

    def stop(self, Input, Output):
        logger.write_in_log("INFO", __name__, "stop", "Game stopped.")
        
        for i in range(4):
            Output.player[i].linearActuator.stop = True

    def check_distance(self,Input):
        winnerId = []
        distance = float('inf')
        for i in range(4):
            new_distance = abs(Input.player[i].linearActuator.currentPose - self.ledCenter)
            if new_distance <= distance:
                distance = new_distance
                winnerId.append(i)
        return winnerId 
        

    def check_move(self, PlayerInput):
        return (
            PlayerInput.gameController.countMove == 0 and
            (PlayerInput.gameController.left or PlayerInput.gameController.right)
        )

    def move(self, PlayerInput, PlayerOutput):
        if self.check_move(PlayerInput):
            PlayerInput.gameController.countMove -= 1
            if PlayerInput.gameController.left:
                PlayerOutput.linearActuator.moveToLeft = True
                PlayerOutput.linearActuator.moveToRight = False
            elif PlayerInput.gameController.right:
                PlayerOutput.linearActuator.moveToLeft = False
                PlayerOutput.linearActuator.moveToRight = True
        else:
            PlayerOutput.linearActuator.moveToLeft = False
            PlayerOutput.linearActuator.moveToRight = False
        
    def check_end_move(self, Input):
        return all(not self.check_move(Input.player[i]) for i in range(4))

    def new_round(self,Input,Output):
        for i in range(4):
            Output.player[i].linearActuator.moveTo = self.ledCenter
        newLedCenter = uniform(Input.player[0].playerLedStrip.rightLimit, Input.player[0].playerLedStrip.leftLimit)
        if newLedCenter == self.ledCenter:
            newPositionLed = uniform(Input.player.playerLedStrip.rightLimit, Input.PlayerInput.playerLedStrip.leftLimit)
        self.ledCenter = newPositionLed
        self.areaLed = [self.ledCenter - self.areaLength/2, self.ledCenter + self.areaLength/2]
        self.actualRound += 1 
        for i in range(4):
            Output.player[i].playerLedStrip.area = self.areaLed
            Output.player[i].linearActuator.stop = False
            Output.player[i].linearActuator.moveToLeft = False
            Output.player[i].linearActuator.moveToRight = False
        Output.PlayerOutput.LinearActuatorOutput.move_to = self.ledCenter
    
        pass

    def modif_score(self,PlayerInput):
        PlayerInput.pointCounter += 1
        pass


    def check_victory(self,Input, Output):
        # for i in range(4):
            # if Input.player[i].pointCounter == 3:
            #     self.winnerID = i
            #     return True
        return False