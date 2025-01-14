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
import subprocess
import time
import uuid
from datetime import datetime
from threading import Thread
from flask import Flask, request, send_from_directory

from ..classes.debug import logFile
log = logFile.LogFile()

app = Flask(__name__)

# Create the log file of the day
log.create_log_file()

PORT_ESP32 = '/dev/ESP32'

# Paths and files
pathDirectory = '/home/pi/python_environnement/bin/activate' # python environment path
pathRepoGithub = '/home/pi/Documents/PING2' # Github repository path

# Define paths for HTML and CSS files
pathHTML = '../Desktop/dossier/dossier/index.html' # Update with the correct path
pathCSS = '../Desktop/dossier/dossier/styles.css'  # Update with the correct path

# Define files to check for Github
filesToCheck = ["README.md", "firmware.bin"] # Update with the correct files (all files to check)
pathPrincipalMain = '/path/to/your/main.py'  # Update with the correct path (start the game)

timeForWifi = 60  # Time to wait for Wi-Fi connection in seconds

# Remove files of network configuration
os.system('sudo rm -rf /etc/NetworkManager/system-connections/*')

# Activate the virtual environment
subprocess.run(['bash', '-c', f'source {pathDirectory}'])

# Function to check Wi-Fi connectivity
def check_wifi():
    try:
        subprocess.run(['ping', '-c', '1', '-W', '1', 'google.com'], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        return True
    except subprocess.CalledProcessError:
        return False

# Function to update Git files
def update_git():
    os.chdir(pathRepoGithub)
    subprocess.run(['git', 'fetch', 'origin'], check=True)
    for file in filesToCheck:
        if subprocess.run(['git', 'diff', '--name-only', 'origin/main'], stdout=subprocess.PIPE).stdout.decode().strip().find(file) != -1:
            # Cloning in progress...
            subprocess.run(['git', 'checkout', 'origin/main', '--', file], check=True)
            log.write_in_log("INFO", "init_rasp", "update_git", 'Git file updated: ')
            # Check if the file is the firmware of the ESP32
            if file == "firmware.bin":
                log.write_in_log("INFO", "init_rasp", "update_git", "ESP32 firmware updated")
                subprocess.run(['esptool.py', '--port', PORT_ESP32, 'write_flash', '-z', '0x0000', 'firmware.bin'], check=True)

# Flask route for Wi-Fi configuration page
@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Retrieve network credentials from form
        ssid = request.form["ssid"]
        password = request.form["password"]
        connection_uuid = str(uuid.uuid4())

        # Create NetworkManager configuration for the network
        ssid = ssid + ' '
        config_content = f"""
[connection]
id={ssid} 
uuid={connection_uuid}
type=wifi
autoconnect=true

[wifi]
ssid={ssid}
mode=infrastructure

[wifi-security]
key-mgmt=wpa-psk
psk={password}

[ipv4]
method=auto

[ipv6]
method=ignore
"""
        config_path = f"/etc/NetworkManager/system-connections/{ssid}.nmconnection"
        with open(config_path, 'w') as f:
            f.write(config_content)
        os.chmod(config_path, 0o600)
    
        # Restart NetworkManager
        time.sleep(5)
        os.system("sudo systemctl restart NetworkManager")
        startingTime = datetime.now()

        #Work with dynamic check loop
        currentTime = datetime.now()
        watchdogTime = 5
        while check_wifi() == 0 and ((currentTime - startingTime).seconds <= watchdogTime):
            currentTime = datetime.now()
        outputLatch = ((currentTime - startingTime).seconds > watchdogTime)
        if outputLatch:
            start_services()
            log.write_in_log("ERROR", "init_rasp", "index", "Wi-Fi configuration failed: Connection could not be established within the timeout period")
        else:
            log.write_in_log("INFO", "init_rasp", "index", "Wi-Fi configuration successful")
            
        return 
    return send_from_directory(os.path.dirname(pathHTML), os.path.basename(pathHTML))  # Serve the HTML file

@app.route("/styles.css")
def styles():
    return send_from_directory(os.path.dirname(pathCSS), os.path.basename(pathCSS))

# Function to start services for Wi-Fi setup
def start_services():
    global timeout
    timeout = time.time() + timeForWifi  # Reset the 60-second timer
    os.system('sudo systemctl stop hostapd')
    os.system('sudo systemctl stop dnsmasq')
    os.system('sudo systemctl start hostapd')
    os.system('sudo systemctl start dnsmasq')
    Thread(target=monitor_services).start()  # Run service monitoring in a separate thread
    log.write_in_log("INFO", "init_rasp", "start_services", "Wi-Fi setup services started")

# Function to stop services after a delay if not connected
def monitor_services():
    global should_stop, timeout
    while time.time() < timeout:
        if check_wifi():
            should_stop = True
            stop_services()  # Stop services if connected
            shutdown_server()  # Stop the Flask server
            log.write_in_log("INFO", "init_rasp", "monitor_services", "Wi-Fi setup successful")
            return
        
        time.sleep(5)  # Check connection
    # Stop services if no connection after timeout
    if not check_wifi():
        stop_services()
        shutdown_server()  # Stop the Flask server

# Function to stop services immediately
def stop_services():
    os.system('sudo systemctl stop hostapd')
    os.system('sudo systemctl stop dnsmasq')

# Function to shutdown the Flask server gracefully
def shutdown_server():
    os._exit(0)  # Forcefully stop the Flask server and end the program

# Main function to handle network logic
def main():
    global should_stop
    should_stop = False
    if check_wifi():
        log.write_in_log("INFO", "init_rasp", "main", "Raspberry Pi is connected to a network")
        update_git()  # Update Git files
        subprocess.run(['python3', pathPrincipalMain])  # Run main.py
    else:
        log.write_in_log("INFO", "init_rasp", "main", "Raspberry Pi is not connected to a network")
        start_services()  # Start services for Wi-Fi setup
        app.run(host='0.0.0.0', port=80)  # Start Flask server

if __name__ == "__main__":
    main()
