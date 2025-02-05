from .gameMode import GameMode
from pingpy.config.config import YELLOW, PATH_AUDIO_SANDBOX_INTRO
from ..output.output import Output
from ..input.input import Input
from pingpy.debug import logger

class SandBox(GameMode):
    """
    Game mode of Sand Box. Bac à sable in french.
    """
    def __init__(self):
        self.color = YELLOW
        self.descriptionAudioPath = PATH_AUDIO_SANDBOX_INTRO
        
    def setup(self, Input, Output):
        """
        Setup the game mode.
        """
                
        if not Input.player or not Output.player:
            logger.write_in_log("ERROR", __name__, "setup", "Input or Output data is missing.")
            return
        
        for i in range(4):
            try:
                playerOutput = Output.player[i]
                playerInput = Input.player[i]
                playerOutput.playerLedStrip.area = [-200, 200] 
                playerOutput.linearActuator.moveTo = 0.0
                playerOutput.linearActuator.setMaxSpeed = 200.0
                playerOutput.linearActuator.setMaxAccel = 200.0
                
            except IndexError:
                logger.write_in_log("ERROR", __name__, "setup", f"No output found for player ID {Input.playerInput[i]}.")

        Output.speaker.audioPiste = None 
        Output.speaker.stop = True
        self.inGame = True
        
        logger.write_in_log("INFO", __name__, "setup", "Setup complete.")
        
    def check_action(self, playerInput, playerOutput):
        """
        Checks the player's action
        """        
        
        if playerInput.gameController.left == True:
            playerOutput.linearActuator.setMaxSpeed = 100.0
            playerOutput.linearActuator.setMaxAccel = 500.0
            playerOutput.linearActuator.moveToLeft = True
            playerInput.gameController.left = False
        if playerInput.gameController.right == True:
            playerOutput.linearActuator.setMaxSpeed = 100.0
            playerOutput.linearActuator.setMaxAccel = 500.0
            playerOutput.linearActuator.moveToRight = True
            playerInput.gameController.right = False            
        if playerInput.gameController.shoot == True:
            playerOutput.bumper.state = True
            playerInput.gameController.shoot = False 
            
        if playerInput.gameController.inAction == False and not(playerInput.gameController.leftButtonState or playerInput.gameController.rightButtonState or playerInput.gameController.shootButtonState):
            playerInput.gameController.inAction = None
            playerOutput.linearActuator.stop = True
                   
        
                
    def compute(self, Input, Output):
        """
        Executes an iteration of the game mode.
        """
        
        if Input.UICorner.resetShortPress:
            Input.UICorner.resetShortPress = None
            self.setup(Input, Output)
            return
                
        if not Input.player:
            logger.write_in_log("ERROR", __name__, "compute", "No players connected...")
            
        if not self.inGame:
            return
            
        if Input.UICorner.level is not None:
            pass
            
        for i in range(4):
            self.check_action(Input.player[i], Output.player[i])
    
    def stop(self, output_ptr):
        """
        Stops the game and resets the outputs.
        """ 
        self.inGame = False
            
        for i in range(4):
            output_ptr.player[i].linearActuator.stop = True            
            
        logger.write_in_log("INFO", __name__, "stop", "Game stopped.")
            
            
            
            