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

from datetime import datetime

pathLogFolder = '/home/pi/Documents/Log_folder' # Path to the log folder

class LogFile:    
    def create_log_file(Date):
        # Create the log folder if it does not exist
        logFilename = f"{pathLogFolder}/Log_file_{Date}.log"
        if not os.path.exists(logFilename):
            print(f"Log file: {logFilename} is created")
            with open(logFilename, 'w') as file:
                file.write(f"---- PING^2 : LOG FILE OF {Date} ----\n")
        else:
            print(f"Log file: {logFilename} already exists")

    def write_in_log(Date, Status, Programme, Function, Message):
        # open the log file of the day
        logFilename = f"{pathLogFolder}/Log_file_{Date}.log"
        with open(logFilename, 'a') as file:
            # write the date
            file.write(str(datetime.now()) + ' ')
            # write the status
            file.write(Status + ' ')
            # write the programme
            file.write(Programme + ' ')
            # write the function
            file.write(Function + ' ')
            # write the message
            file.write(Message + '\n')
    
