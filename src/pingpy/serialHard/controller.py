from serial import Serial

class ControllerSerial(Serial):
    def __init__(self):
        print("Controller class created")