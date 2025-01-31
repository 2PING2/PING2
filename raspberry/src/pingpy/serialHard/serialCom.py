# """
# This file is part of the PING² project.
# Copyright (c) 2024 PING² Team

# This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International (CC BY-NC-ND 4.0).
# You may share this file as long as you credit the original author.

# RESTRICTIONS:
# - Commercial use is prohibited.
# - No modifications or adaptations are allowed.
# - See the full license at: https://creativecommons.org/licenses/by-nc-nd/4.0/

# For inquiries, contact us at: projet.ping2@gmail.com
# """

# import serial
# import time
# import os
# from threading import Thread
# from pingpy.config.config import RETRY_ATTEMPTS, RETRY_DELAY
# from pingpy.debug import logger
# from serial.tools import list_ports  # pyserial

# ''' Communication class useful for the serial communication between the Raspberry Pi and other devices. '''
# class SerialCom:
#     def __init__(self, symlink, BAUD_RATE, TIMEOUT):
#         """Initialize the serial port handler."""
#         self.symlink = symlink
#         self.port = None
#         self.baudrate = BAUD_RATE
#         self.timeout = TIMEOUT
#         self.ser = None
#         self.connected = False
#         self.queue = []
#         self.failed_attempts = 0
#         logger.write_in_log("INFO", __name__, "__init__", f"SerialCom constructed for port {self.symlink}")

#     def setup(self):
#         """Try to open the serial port."""
#         if self.ser:
#             return
#         # Check if the port exists
#         if not os.path.exists(self.symlink):
#             self.ser = None
#             # logger.write_in_log("WARNING", __name__, "open_port", f"symlink {self.symlink} does not exist.")
#             return
        


#         # Open the port
#         try:
#             self.ser = serial.Serial(self.symlink, self.baudrate, timeout=self.timeout)
#             # make sure the Serial is closed at the beginning
#             if not self.ser.is_open:
#                 self.ser.open()
#             logger.write_in_log("INFO", __name__, "setup", f"Connected to symlink {self.symlink} at {self.baudrate} baud")   
#         except Exception as e:
#             logger.write_in_log("ERROR", __name__, "setup", f"Error opening port {self.symlink}: {e}")
#             return          

    
#     def read_data_task(self, onDisconnect = None, args = None):
#         """Read the next data from the serial port."""
#         if os.path.exists(self.symlink):
#             try:
#                 self.connected = True
#                 if self.ser.in_waiting == 0:
#                     return
#                 new = self.ser.readline().decode('utf-8', errors='ignore').strip()
#                 logger.write_in_log("INFO", __name__, "read_data_task", f"Data received from {self.symlink}: {new}")
#                 self.queue.append(new)
#             except Exception as _:
#                 self.ser=None
#                 self.setup()
#         else:
#             self.ser=None
#             if self.connected:
#                 self.connected = False
#                 # if callable(onDisconnect):  # Vérification si onDisconnect est une fonction
#                 #     try:
#                 #         onDisconnect()
#                 #     except Exception as e:
#                 #         logger.write_in_log("ERROR", __name__, "read_data_task", f"Error in onDisconnect callback: {e}")
#                 logger.write_in_log("WARNING", __name__, "read_data_task", f"symlink {self.symlink} does not exist.")
                
            
        
#     def send_data(self, data):
#         if not os.path.exists(self.symlink):
#             return
#         try:
#             self.ser.write(data.encode() + b'\n')
#             logger.write_in_log("INFO", __name__, "send_data", f"Data sent to {self.symlink}: {data}")
#         except serial.SerialException as e:
#             logger.write_in_log("ERROR", __name__, "send_data", f"Error sending data to {self.symlink}: {e}")
#             self.connected = False
            
#     def consume_older_data(self):
#         """Consume the older data in the queue."""
#         if self.queue:
#             return self.queue.pop(0)
#         else:
#             return None
            
#     def stop_reading(self):
#         """Stop reading from the serial port."""
#         self.connected = False
#         if self.ser:
#             self.ser.close()
#             logger.write_in_log("INFO", "SerialPortHandler", "stop_reading", f"Reading stopped on {self.symlink}")
            
            
import serial
import time
import os
from threading import Thread
from pingpy.config.config import RETRY_ATTEMPTS, RETRY_DELAY
from pingpy.debug import logger
from serial.tools import list_ports  # pyserial

''' Communication class useful for the serial communication between the Raspberry Pi and other devices. '''
class SerialCom:
    def __init__(self, symlink, BAUD_RATE, TIMEOUT):
        """Initialize the serial port handler."""
        self.symlink = symlink
        self.port = None
        self.baudrate = BAUD_RATE
        self.timeout = TIMEOUT
        self.ser = None
        self.connected = False
        self.disconnectFlag = False
        self.queue = []
        self.failed_attempts = 0
        logger.write_in_log("INFO", __name__, "__init__", f"SerialCom constructed for port {self.symlink}")

    def setup(self):
        """Try to open the serial port."""
        if self.ser:
            return
        # Check if the port exists
        if not os.path.exists(self.symlink):
            self.ser = None
            # logger.write_in_log("WARNING", __name__, "open_port", f"symlink {self.symlink} does not exist.")
            return



        # Open the port
        try:
            self.ser = serial.Serial(self.symlink, self.baudrate, timeout=self.timeout)
            # make sure the Serial is closed at the beginning
            if not self.ser.is_open:
                self.ser.open()
            logger.write_in_log("INFO", __name__, "setup", f"Connected to symlink {self.symlink} at {self.baudrate} baud")   
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "setup", f"Error opening port {self.symlink}: {e}")
            return          


    def read_data_task(self):
        """Read the next data from the serial port."""
        if os.path.exists(self.symlink):
            try:
                self.disconnectFlag = False
                if self.ser.in_waiting == 0:
                    return
                new = self.ser.readline().decode('utf-8', errors='ignore').strip()
                logger.write_in_log("INFO", __name__, "read_data_task", f"Data received from {self.symlink}: {new}")
                self.queue.append(new)
            except Exception as _:
                self.ser=None
                self.setup()
        else:
            self.ser=None
            self.connected = False
            if not self.disconnectFlag:
                self.disconnectFlag = True
                logger.write_in_log("WARNING", __name__, "read_data_task", f"symlink {self.symlink} does not exist.")
            
    def send_data(self, data):
        logger.write_in_log("INFO", __name__, "send_data", f"test {data}")
        if not os.path.exists(self.symlink):
            return
        logger.write_in_log("INFO", __name__, "send_data", f"Data sent to {self.symlink}: {data}")
        try:
            self.ser.write(data.encode() + b'\n')
            logger.write_in_log("INFO", __name__, "send_data", f"Data sent to {self.symlink}: {data}")
        except serial.SerialException as e:
            logger.write_in_log("ERROR", __name__, "send_data", f"Error sending data to {self.symlink}: {e}")
            self.connected = False

    def consume_older_data(self):
        """Consume the older data in the queue."""
        if self.queue:
            return self.queue.pop(0)
        else:
            return None

    def stop_reading(self):
        """Stop reading from the serial port."""
        self.connected = False
        if self.ser:
            self.ser.close()
            logger.write_in_log("INFO", "SerialPortHandler", "stop_reading", f"Reading stopped on {self.symlink}")