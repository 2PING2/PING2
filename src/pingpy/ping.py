from pingpy import *

class Ping:
    def __init__(self):
        self.esp32 = serialHard.ESP32Serial()
        self.UICorner = serialHard.UICornerSerial()
        print("Ping class created")