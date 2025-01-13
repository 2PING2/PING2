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
import threading
import subprocess
import time
import os
from ....config.config import RETRY_ATTEMPTS, RETRY_DELAY, MAX_BRIGTHNESS
from classes.debug.logFile import LogFile
# Initialize the logs
log = LogFile()

''' Communication class useful for the serial communication between the Raspberry Pi and other devices. '''
class Serial:
    def __init__(self, port, BAUD_RATE, TIMEOUT):
        """Initialize the serial port handler."""
        self.port = port
        self.baudrate = BAUD_RATE
        self.timeout = TIMEOUT
        self.ser = None
        self.running = False
        self.lastData = None
        self.retryCount = 0
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
                if data and data != self.lastData:
                    self.lastData = data
                    self.process_data(self.lastData)                   
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