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
import pyudev
context = pyudev.Context()


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
        logger.write_in_log("INFO", __name__, "__init__", f"SerialCom constructed for port {self.symlink}")

    def setup(self):
        """Configure and start reading the serial port."""
        self.ser = self.open_port()
        if self.ser:
            self.connected = True
            self.port = os.path.realpath(self.symlink)
            logger.write_in_log("INFO", __name__, "setup", f"Reading started on {self.symlink}")
        else:
            self.connected = False
            self.port = None
            logger.write_in_log("WARNING", __name__, "setup", f"Symlink {self.symlink} not connected or not accessible after {RETRY_ATTEMPTS} attempts.")

    def open_port(self):
        """Try to open the serial port."""
        for _ in range(RETRY_ATTEMPTS):
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

    def read_data_task(self):
        """Read the next data from the serial port."""
        self.check_usb_event()
        if not self.connected:
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
            
        except Exception as e:
            logger.write_in_log("ERROR", __name__, "read_data", f"Error processing data from {self.symlink}:  {e}")
            self.connected = False
        # time.sleep(0.01)
            
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
        wasConnected = self.connected
        self.connected = False
        # connectedUsb = os.listdir('/dev/')
        connectedUsb = []
        for device in context.list_devices(subsystem='tty'):
            connectedUsb.append(device.device_node)
        print(connectedUsb)
        
        for port in connectedUsb:
            if '/dev/'+port == self.symlink:

                self.connected = True
                break
            
        if not wasConnected and self.connected:
            logger.write_in_log("INFO", __name__, "check_usb_event", f"Reconnected to {self.symlink}")
            self.setup()
            
        if wasConnected and not self.connected:
            logger.write_in_log("WARNING", __name__, "check_usb_event", f"Disconnected from {self.symlink}")
            self.stop_reading()
            
        # time.sleep(1)
        
            
