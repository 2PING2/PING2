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

import os
from datetime import datetime

pathLogFolder = "/home/pi/Documents/logFolder" # Path to the log folder

class LogFile:
    def __init__(self, log_folder=pathLogFolder):
        self.logFolder = log_folder
        # Create the log folder if it does not exist
        os.makedirs(self.logFolder, exist_ok=True)

    def create_log_file(self):
        today = datetime.now().strftime("%d-%m-%Y")
        logFilename = os.path.join(self.logFolder, f"Log_file_{today}.log")
        if not os.path.exists(logFilename):
            print(f"Log file: {logFilename} is created")
            with open(logFilename, 'w') as file:
                file.write(f"---- PING^2 : LOG FILE OF {today} ----\n")
        else:
            print(f"Log file: {logFilename} already exists")

    def write_in_log(self, status, programme, function, message):
        today = datetime.now().strftime("%d-%m-%Y")      
        logFilename = os.path.join(self.logFolder, f"Log_file_{today}.log")
        try:
            with open(logFilename, 'a') as file:
                file.write(f"{datetime.now().strftime('%d-%m-%Y %H:%M:%S.%f')[:-3]} {status} {programme} {function} {message}\n")

        except Exception as e:
            print(f"Failed to write in log file {logFilename}: {e}")

if __name__ == "__main__":    
    # Init the log handler
    log = LogFile()

    # Create the log file
    log.create_log_file()
    
    # Write in log (examples)
    log.write_in_log("INFO", "MainProgram", "InitFunction", "Application started successfully.")
    log.write_in_log("ERROR", "init_rasp", "index", "Wi-Fi configuration failed")
    log.write_in_log("DEBUG", "MainProgram", "ComputeFunction", "The result is xx.")