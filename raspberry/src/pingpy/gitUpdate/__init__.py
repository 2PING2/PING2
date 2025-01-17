from .hotspot import Hotspot
from pingpy.debug import logger
import uuid
import time
import os
from datetime import datetime
from flask import Flask, request, send_from_directory
from pingpy.config.config import HTML_PATH, CSS_PATH

app = Flask(__name__)

hotspot = Hotspot()

@app.route("/", methods=["GET", "POST"])
def index():
    if request.method == "POST":
        # Retrieve network credentials from form
        ssid = request.form["ssid"]
        password = request.form["password"]
        connection_uuid = str(uuid.uuid4())

        # Create NetworkManager configuration for the network
        # ssid = ssid + ' '
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
        while hotspot.check_wifi() == 0 and ((currentTime - startingTime).seconds <= watchdogTime):
            currentTime = datetime.now()
        outputLatch = ((currentTime - startingTime).seconds > watchdogTime)
        if outputLatch:
            hotspot.start_services()
            logger.write_in_log("ERROR", __name__, "index", "Wi-Fi configuration failed: Connection could not be established within the timeout period")
        else:
            logger.write_in_log("INFO", __name__, "index", "Wi-Fi configuration successful")
            
        return 
    if os.path.exists(HTML_PATH):
        logger.write_in_log("INFO", __name__, "index", "Serving HTML file")
    else:
        logger.write_in_log("ERROR", __name__, "index", "HTML file not found")
        
    return send_from_directory(os.path.dirname(HTML_PATH), os.path.basename(HTML_PATH))  # Serve the HTML file

@app.route("/styles.css")
def styles():
    if os.path.exists(CSS_PATH):
        logger.write_in_log("INFO", __name__, "styles", "Serving CSS file")
    else:
        logger.write_in_log("ERROR", __name__, "styles", "CSS file not found")
    return send_from_directory(os.path.dirname(CSS_PATH), os.path.basename(CSS_PATH))

hotspot.setup(app)
# from threading import Thread 
# Thread(target=hotspot.setup, args=(app,)).start()