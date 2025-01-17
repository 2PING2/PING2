from pingpy.debug import logger
from pingpy.config.config import PORT_ESP32, FILE_AND_FOLDER_TO_CHECK, ESP_FIRMWARE_PATH, GIT_CLONE_PATH, HOTSPOT_TIMEOUT, CHECK_WIFI_DELAY, GIT_BRANCH, ROOT_PATH, ESP_BOOTLOADER_PATH, ESP_PARTITION_PATH
import os
import subprocess
import time


from threading import Thread


class Hotspot:
    def __init__(self):
        self.should_stop = False
        self.timeout = None
        logger.write_in_log("INFO", __name__, "__init__")
        
    def setup(self, app):
        if self.check_wifi():
            self.check_git_update()
        else:
            self.start_services()
            # app.run(host='0.0.0.0', port=80) 
            # make a thread to run the app.run
            Thread(target=app.run, kwargs={'host': '0.0.0.0', 'port':80}).start()
        logger.write_in_log("INFO", __name__, "setup")

    def check_wifi(self):
        try:
            subprocess.run(['ping', '-c', '1', '-W', '1', 'google.com'], check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            logger.write_in_log("INFO", __name__, "check_wifi", "Wi-Fi connected")
            return True
        except subprocess.CalledProcessError:
            logger.write_in_log("WARNING", __name__, "check_wifi", "Wi-Fi not connected")
            return False
        
    def check_git_update(self):
        os.chdir(GIT_CLONE_PATH)
        restartNeeded = False
        try:
            subprocess.run(['git', 'fetch', 'origin'], check=True)
            logger.write_in_log("INFO", __name__, "check_git_update", "Git fetch successful")
        except subprocess.CalledProcessError:
            logger.write_in_log("ERROR", __name__, "check_git_update", "Git fetch failed")
            return
        
        for fileOrFolder in FILE_AND_FOLDER_TO_CHECK:
            try:
                # Vérifie les différences pour le fichier ou dossier
                diff_output = subprocess.run(
                ['git', 'diff', '--name-only', GIT_BRANCH, fileOrFolder],
                check=True,
                stdout=subprocess.PIPE,  # Capture la sortie pour vérification
                text=True  # Renvoie la sortie sous forme de chaîne
                ).stdout.strip()
                
                # S'il y a une différence, effectue le checkout et marque le redémarrage nécessaire
                if diff_output:  # Si la sortie n'est pas vide
                    subprocess.run(['git', 'checkout', GIT_BRANCH, '--', fileOrFolder], check=True)
                    logger.write_in_log("INFO", __name__, "check_git_update", f'Git file updated: {fileOrFolder}')
                    if fileOrFolder == ESP_FIRMWARE_PATH or fileOrFolder == ESP_BOOTLOADER_PATH or fileOrFolder == ESP_PARTITION_PATH:
                        self.update_esp()
                    restartNeeded = True  # Indique qu'un redémarrage est nécessaire
                            
            except subprocess.CalledProcessError:
                logger.write_in_log("ERROR", __name__, "check_git_update", f'Git file not updated: {fileOrFolder}')
            
                
        # restart the app if needed
        if restartNeeded:
            logger.write_in_log("INFO", __name__, "check_git_update", "Restarting app")
            os.system(f'sleep 0.1 && python3 /home/pi/Documents/PING2/raspberry/src/main.py')
            exit(1) 

    def build_backup(self):
        backup_dir = os.path.join(ROOT_PATH, "backup")
        os.makedirs(backup_dir, exist_ok=True)
        # copy the concent of PIN2 folder to the backup folder
        os.system(f'cp -r {GIT_CLONE_PATH}/* {backup_dir}')
        
                
    def update_esp(self):
        try:
            # subprocess.run(['esptool.py', '--chip esp32 --port', PORT_ESP32 ,'--baud 115200 write_flash 0x1000 ',ESP_BOOTLOADER_PATH , '0x8000 ',ESP_PARTITION_PATH,' 0x10000', ESP_FIRMWARE_PATH], check=True)
            subprocess.run([
            'esptool.py',
            '--chip', 'esp32',
            '--port', PORT_ESP32,
            '--baud', '115200',
            'write_flash',
            '0x1000', ESP_BOOTLOADER_PATH,
            '0x8000', ESP_PARTITION_PATH,
            '0x10000', ESP_FIRMWARE_PATH
        ], check=True)

            logger.write_in_log("INFO", __name__, "update_esp", "success")
        except subprocess.CalledProcessError:
            logger.write_in_log("ERROR", __name__, "update_esp", "fail")
            
    def start_services(self):
        self.timeout = time.time() + HOTSPOT_TIMEOUT  # Set timeout to 5 minutes
        os.system('sudo systemctl stop hostapd')
        os.system('sudo systemctl stop dnsmasq')
        os.system('sudo systemctl start hostapd')
        os.system('sudo systemctl start dnsmasq')
        Thread(target=self.monitor_services).start()  # Run service monitoring in a separate thread
        logger.write_in_log("INFO", __name__, "start_services")
        
    def monitor_services(self):
        while time.time() < self.timeout:
            if self.check_wifi():
                self.should_stop = True
                self.stop_services()  # Stop services if connected
                self.check_git_update()
                return
            time.sleep(CHECK_WIFI_DELAY)  # Check connection
        # Stop services if no connection after timeout
        if not self.check_wifi():
            self.stop_services()
            logger.write_in_log("INFO", __name__, "monitor_services", "Hotspot timeout reached")

    def stop_services(self):
        os.system('sudo systemctl stop hostapd')
        os.system('sudo systemctl stop dnsmasq')
        logger.write_in_log("INFO", __name__, "stop_services")
        
