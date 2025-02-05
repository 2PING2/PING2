from .serialCom import SerialCom
from pingpy.debug import logger
from pingpy.config.config import PLAYER_KEY, PARAM_BEGIN_SEP, PARAM_END_SEP, KEY_SEP, MOVE_TO_LEFT_LIMIT_KEY, MOVE_TO_RIGHT_LIMIT_KEY, STOP_KEY, SET_MAX_SPEED_KEY, CURRENT_SPEED_KEY, CURRENT_POSITION_KEY, RIGHT_LIMIT_KEY, LEFT_LIMIT_KEY, CALIBRATION_KEY, SET_SOL_STATE_KEY, MOVE_TO_KEY
  
class ESP32Serial(SerialCom):
    def __init__(self, port, baudrate, timeout):
        super().__init__(port, baudrate, timeout)
        logger.write_in_log("INFO", __name__, "__init__")
        self.key_values = []
        
    def setup(self, output_ptr):
        super().setup()
        # OUTPUT lpayer -> ask calibration = True
        for playerOutput in output_ptr.player:
            playerOutput.linearActuator.askForCalibration = True

        
    def read(self, input_ptr):
        """Read the next data from the serial port."""
        self.read_data_task()
        new_line = self.consume_older_data()
        
        if new_line is None:
            return
            
        key = ""
        param = ""
        is_param = False
        self.key_values = []

        for i in range(len(new_line)):
            char = new_line[i]
            if char == KEY_SEP or i == len(new_line) - 1:
                # Store the key-value pair
                self.key_values.append({"key": key, "param": param})
                key = ""
                param = ""
            elif char == PARAM_BEGIN_SEP:
                is_param = True
            elif char == PARAM_END_SEP:
                is_param = False
            elif is_param:
                param += char
            else:
                key += char
            
        self.process_key_values(input_ptr)

    def process_key_values(self, input_ptr):
        """Processes the parsed key-value pairs."""
        # Replace this method with your own logic
        # for kv in self.key_values:
        #     logger.write_in_log("INFO", __name__, "process_key_values", f"Key: {kv['key']}, Param: {kv['param']}")
        if len(self.key_values) < 2:
            logger.write_in_log("ERROR", __name__, "process_key_values", "Not enough key-values.")
            return
        
        if self.key_values[0]['key'] != PLAYER_KEY:
            return
        # get player id
        player_id = int(self.key_values[0]['param'])-1
        playerInput = input_ptr.player[player_id]
        if self.key_values[1]['key'] == CURRENT_SPEED_KEY:
            playerInput.linearActuator.currentSpeed = float(self.key_values[1]['param'])
            if playerInput.linearActuator.currentSpeed == 0:
                playerInput.linearActuator.moving = False
            else:
                playerInput.linearActuator.moving = True
                
        elif self.key_values[1]['key'] == CURRENT_POSITION_KEY:
            playerInput.linearActuator.currentPose = float(self.key_values[1]['param'])
            logger.write_in_log("INFO", __name__, "process_key_values", f"Current position: {playerInput.linearActuator.currentPose}")
        elif self.key_values[1]['key'] == RIGHT_LIMIT_KEY:
            playerInput.linearActuator.rightLimit = float(self.key_values[1]['param'])
            logger.write_in_log("INFO", __name__, "process_key_values", f"Right limit: {playerInput.linearActuator.rightLimit}")
        elif self.key_values[1]['key'] == LEFT_LIMIT_KEY:
            playerInput.linearActuator.leftLimit = float(self.key_values[1]['param'])
            logger.write_in_log("INFO", __name__, "process_key_values", f"Left limit: {playerInput.linearActuator.leftLimit}")
            
            
        
        
    def write(self, output_ptr, input_ptr):
        """Write the next data to the serial port."""
        for i in range(len(output_ptr.player)):
            playerOutput = output_ptr.player[i]
            playerInput = input_ptr.player[i]
            if playerOutput.linearActuator.moveToRight:
                playerInput.linearActuator.moving = True
                playerOutput.linearActuator.moveToRight = None
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i+1) + PARAM_END_SEP + KEY_SEP + MOVE_TO_RIGHT_LIMIT_KEY)
            if playerOutput.linearActuator.moveToLeft:
                playerInput.linearActuator.moving = True
                playerOutput.linearActuator.moveToLeft = None
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i+1) + PARAM_END_SEP + KEY_SEP + MOVE_TO_LEFT_LIMIT_KEY)
            if playerOutput.linearActuator.stop:
                playerOutput.linearActuator.stop = None
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i+1) + PARAM_END_SEP + KEY_SEP + STOP_KEY)
            if playerOutput.linearActuator.setMaxSpeed:
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i+1) + PARAM_END_SEP + KEY_SEP + SET_MAX_SPEED_KEY + PARAM_BEGIN_SEP + str(playerOutput.linearActuator.setMaxSpeed) + PARAM_END_SEP)
                playerOutput.linearActuator.setMaxSpeed = None
            if playerOutput.linearActuator.askForCalibration:
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i+1) + PARAM_END_SEP + KEY_SEP + CALIBRATION_KEY)
                playerOutput.linearActuator.askForCalibration = None
            if playerOutput.linearActuator.moveTo is not None:
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i+1) + PARAM_END_SEP + KEY_SEP + MOVE_TO_KEY + PARAM_BEGIN_SEP + str(playerOutput.linearActuator.moveTo) + PARAM_END_SEP)
                playerOutput.linearActuator.moveTo = None
            if playerOutput.bumper.state is not None:
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i+1) + PARAM_END_SEP + KEY_SEP + SET_SOL_STATE_KEY + PARAM_BEGIN_SEP + str(playerOutput.bumper.state) + PARAM_END_SEP)
                playerOutput.bumper.state = None