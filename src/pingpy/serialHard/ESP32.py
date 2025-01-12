from serial import Serial
  
class ESP32Serial(Serial):
    def __init__(self):
        print("ESP32 class created")