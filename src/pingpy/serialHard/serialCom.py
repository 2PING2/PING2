"""
This file is part of the PING² project.
Copyright (c) 2024 PING² Team

This code is licensed under the Attribution-NonCommercial-NoDerivatives 4.0 International (CC BY-NC-ND 4.0).
You may share this file as long as you credit the original author.

RESTRICTIONS:
- Commercial use is prohibited.
- No modifications or adaptations are allowed.
- See the full license at: https://creativecommons.org/licenses/by-nc-nd/4.0/

For inquiries, contact us at: projet.ping2@gmail.com
"""

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
        self.retryCount = 0
        self.queue = []
        self.attemptsIndex = 0
        self.lastAttemptTime = time.time()
        logger.write_in_log("INFO", __name__, "__init__", f"SerialCom constructed for port {self.symlink}")

    def setup(self):
        """Configure and start reading the serial port."""
        try:
            ret = self.open_port()
            if ret == 0:
                return
            elif ret == -1:
                logger.write_in_log("ERROR", __name__, "setup", f"Error opening port {self.symlink} after {RETRY_ATTEMPTS} attempts.")
            self.ser = ret
            if ret is None:
                return
            
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "setup", f"Error opening port {self.symlink}: {e}")
        try:
            if self.ser is not None:
                if self.ser:
                    self.connected = True
                    try:
                        self.port = os.path.realpath(self.symlink)
                        logger.write_in_log("INFO", __name__, "setup", f"Reading started on {self.symlink}")
                        return
                    except Exception as e:
                        self.connected = False
                        self.port = None
                        logger.write_in_log("ERROR", __name__, "setup", f"Error getting the real path of {self.symlink}: {e}")
                        return
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "setup", f"Error starting reading on {self.symlink}: {e}")
        try:
            self.connected = False
            self.port = None
            logger.write_in_log("WARNING", __name__, "setup", f"Symlink {self.symlink} not connected or not accessible after {RETRY_ATTEMPTS} attempts.")
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "setup", f"Error setting up {self.symlink}: {e}")

    def open_port(self):
        """Try to open the serial port."""
        if time.time() - self.lastAttemptTime > RETRY_DELAY:
            return 0 # return that we did not try to connect to the port
        
        if self.attemptsIndex >= RETRY_ATTEMPTS:
            return -1 # return that we tried to connect to the port but failed
        
        self.attemptsIndex+=1
        try:
            # Check if the port exists
            if not os.path.exists(self.symlink):
                logger.write_in_log("WARNING", __name__, "open_port", f"symlink {self.symlink} does not exist.")
                return None

            # Try to open the port
            ser = serial.Serial(self.symlink, self.baudrate, timeout=self.timeout)
            # ser.reset_input_buffer()
            # ser.set_buffer_size(rx_size = 4096, tx_size = 4096)
            # make sure the Serial is closed at the beginning
            ser.close()
            ser.open()
            logger.write_in_log("INFO", __name__, "open_port", f"Connected to symlink {self.symlink} at {self.baudrate} baud")
            # begin asynchronous reading
            # Thread(target=self.read_data_task, daemon = True).start()
            return ser
        except serial.SerialException as e:
            logger.write_in_log("ERROR", __name__, "open_port", f"Error connecting to symlink {self.symlink}: {e}")
            time.sleep(RETRY_DELAY)
            return None
    
        

        # for _ in range(RETRY_ATTEMPTS):
            # try:
            #     # Check if the port exists
            #     if not os.path.exists(self.symlink):
            #         logger.write_in_log("WARNING", __name__, "open_port", f"symlink {self.symlink} does not exist.")
            #         return None

            #     # Try to open the port
            #     ser = serial.Serial(self.symlink, self.baudrate, timeout=self.timeout)
            #     # ser.reset_input_buffer()
            #     # ser.set_buffer_size(rx_size = 4096, tx_size = 4096)
            #     # make sure the Serial is closed at the beginning
            #     ser.close()
            #     ser.open()
            #     logger.write_in_log("INFO", __name__, "open_port", f"Connected to symlink {self.symlink} at {self.baudrate} baud")
            #     # begin asynchronous reading
            #     # Thread(target=self.read_data_task, daemon = True).start()
            #     return ser
            # except serial.SerialException as e:
            #     logger.write_in_log("ERROR", __name__, "open_port", f"Error connecting to symlink {self.symlink}: {e}")
            #     time.sleep(RETRY_DELAY)
        
        # return None

    def read_data_task(self):
        """Read the next data from the serial port."""
        try:
            self.check_usb_event()
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read_data_task", f"Error in check_usb_event: {e}")
            return
        
        if not self.connected:
            return
        if self.ser is None:
            return
        
        try:
            if self.ser.in_waiting > 0:
                new = self.ser.readline().decode('utf-8', errors='ignore').strip()
            else:
                new = False
            if new:
                logger.write_in_log("INFO", __name__, "read_data", f"Data received from {self.symlink}: {new}")
                self.queue.append(new)
                
        except serial.SerialException as e:
            logger.write_in_log("ERROR", __name__, "read_data", f"Error reading from {self.symlink}: {e}")
            self.connected = False
            return
            
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read_data", f"Error processing data from {self.symlink}:  {e}")
            self.connected = False
            return
        # time.sleep(0.01)
        
    def send_data(self, data):
        if not self.connected:
            return
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
            
    def check_usb_event(self):
        """Check if a USB device is connected or disconnected."""
        try:
            if self.connected is None:
                self.connected = False
            wasConnected = self.connected
            self.connected = False #  a tester ? os.path.exists(self.symlink)
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "check_usb_event", f"Error checking {self.symlink} existence: {e}")
        try:
            connectedUsb = os.listdir('/dev/')
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "check_usb_event", f"Error listing /dev/: {e}")
            return
        try:
            for port in connectedUsb:
                if '/dev/'+port == self.symlink:
                    self.connected = True
                    break
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "check_usb_event", f"Error checking {self.symlink} in /dev/: {e}")
            return
        
        try:
            if not wasConnected and self.connected:
                logger.write_in_log("INFO", __name__, "check_usb_event", f"Reconnected to {self.symlink}")
                self.setup()
            elif wasConnected and not self.connected:
                logger.write_in_log("WARNING", __name__, "check_usb_event", f"Disconnected from {self.symlink}")
                self.stop_reading()
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "check_usb_event", f"Error handling {self.symlink} connection: {e}")
            return
            
        # time.sleep(1)
        
            
