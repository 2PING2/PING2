from .serialCom import SerialCom
from pingpy.debug import logger
from pingpy.config.config import PLAYER_KEY, PARAM_BEGIN_SEP, PARAM_END_SEP, KEY_SEP, MOVE_TO_LEFT_LIMIT_KEY, MOVE_TO_RIGHT_LIMIT_KEY, STOP_KEY, SET_MAX_SPEED_KEY
  
class ESP32Serial(SerialCom):
    def __init__(self, port, baudrate, timeout):
        super().__init__(port, baudrate, timeout)
        logger.write_in_log("INFO", __name__, "__init__")
        self.key_values = []
        
    def read(self, input_ptr):
        """Read the next data from the serial port."""
        self.read_data_task()
        new_line = self.consume_older_data()
        
        if new_line is None:
            return
            
        key = ""
        param = ""
        is_param = False

        for char in new_line:
            if char == self.key_sep or char == '\0':
                # Store the key-value pair
                self.key_values.append({"key": key, "param": param})
                key = ""
                param = ""
            elif char == self.param_begin_sep:
                is_param = True
            elif char == self.param_end_sep:
                is_param = False
            elif is_param:
                param += char
            else:
                key += char
            
            self.process_key_values()

    def process_key_values(self):
        """Processes the parsed key-value pairs."""
        # Replace this method with your own logic
        for kv in self.key_values:
            logger.write_in_log("INFO", __name__, "process_key_values", f"Key: {kv['key']}, Param: {kv['param']}")
        
        
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
            if playerOutput.linearActuator.setSpeed:
                self.send_data(PLAYER_KEY + PARAM_BEGIN_SEP + str(i+1) + PARAM_END_SEP + KEY_SEP + SET_MAX_SPEED_KEY + PARAM_BEGIN_SEP + str(playerOutput.linearActuator.setSpeed) + PARAM_END_SEP)
                playerOutput.linearActuator.setSpeed = None
            
            # lorsqu'on demande la current pose
            # if playerOutput.linearActuator. 
                # pass
