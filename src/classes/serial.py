import serial
import threading
import subprocess
import time
import os
from config import RETRY_ATTEMPTS, RETRY_DELAY, MAX_BRIGTHNESS
from logFile import LogFile

# Initialize the logs
log = LogFile()

ports = {
    "UI_Corner": "/dev/ttyS0",
    "Player1": "/dev/ttyUSB1",
    "Player2": "/dev/ttyUSB2",
    "Player3": "/dev/ttyUSB3",
    "Player4": "/dev/ttyUSB4",
    "ESP32": "/dev/ttyUSB5"
}


class Serial:
    def __init__(self, port, BAUD_RATE, TIMEOUT):
        """Initialize the serial port handler."""
        self.port = port
        self.baudrate = BAUD_RATE
        self.timeout = TIMEOUT
        self.ser = None
        self.running = False
        self.last_data = None
        self.retry_count = 0
        log.write_in_log("INFO", "SerialPortHandler", "__init__", f"SerialPortHandler initialized for port {self.port}")

    def setup(self):
        """Configure and start reading the serial port."""
        self.ser = self.open_port()
        if self.ser:
            self.running = True
            threading.Thread(target=self.read_data, daemon=True).start()
            log.write_in_log("INFO", "SerialPortHandler", "setup", f"Reading started on {self.port}")
        else:
            log.write_in_log("WARNING", "SerialPortHandler", "setup", f"Port {self.port} not connected or not accessible after {RETRY_ATTEMPTS} attempts.")

    def open_port(self):
        """Try to open the serial port."""
        for attempt in range(RETRY_ATTEMPTS):
            try:
                # Check if the port exists
                if not os.path.exists(self.port):
                    log.write_in_log("WARNING", "SerialPortHandler", "open_port", f"Port {self.port} does not exist.")
                    return None

                # Try to open the port
                ser = serial.Serial(self.port, self.baudrate, timeout=self.timeout)
                log.write_in_log("INFO", "SerialPortHandler", "open_port", f"Connected to port {self.port}")
                return ser
            except serial.SerialException as e:
                log.write_in_log("ERROR", "SerialPortHandler", "open_port", f"Error connecting to port {self.port}: (Exception)")
                time.sleep(RETRY_DELAY)
        return None

    def read_data(self):
        """Read data from the serial port."""
        while self.running:
            try:
                data = self.ser.readline().decode('utf-8').strip()
                if data and data != self.last_data:
                    self.last_data = data
                    self.process_data(self.last_data)                   
            except serial.SerialException as e:
                log.write_in_log("ERROR", "SerialPortHandler", "read_data", f"Error reading from {self.port}: (Exception)")
                self.running = False
                break
            except Exception as e:
                log.write_in_log("ERROR", "SerialPortHandler", "read_data", f"Error processing data from {self.port}: (Exception)")
                self.running = False
                break

    def stop_reading(self):
        """Stop reading from the serial port."""
        self.running = False
        if self.ser:
            self.ser.close()
            log.write_in_log("INFO", "SerialPortHandler", "stop_reading", f"Reading stopped on {self.port}")


class UICornerSerial(Serial):
    def process_data(self, last_data):
        """ Process the data specific to the UI corner."""
        if "volume" in last_data:
            volume_received = last_data.split(":")[1]
            volume_new = int((volume_received / 1023) * 100)
            try:
                subprocess.run(["amixer", "sset", "Master", f"{volume_new}%"], check=True)
            except subprocess.CalledProcessError as e:
                log.write_in_log("ERROR", "UICorner", "set_volume", "Error setting volume")

        elif "level" in last_data:
            level_received = last_data.split(":")[1]
            # Implement logic to process level data
            
        elif "light" in last_data:
            brightness_received = last_data.split(":")[1]
            brightness_new = int((brightness_received / 1023) * MAX_BRIGTHNESS)
            # Implement logic to process brightness data
            
        elif "reset" in last_data:
            # depend push / realease
            pass
        
        elif "mode_pb" in last_data:
            # depend push / realease
            pass
        
        elif "mode" in last_data:
            # depend increment / decrement
            pass
        
class ControlerSerial(Serial):
    def process_data(self, last_data):
        """ Process the data specific to the controler."""
        if "right" in last_data:
            # depend push / realease
            pass
        if "left" in last_data:
            # depend push / realease
            pass
        if "shoot" in last_data:
            # depend push / realease
            pass
        
class ESP32Serial(Serial):
    def process_data(self, last_data):
        """ Process the data specific to the ESP32."""
        pass