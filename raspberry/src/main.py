try :
    # from pingpy import ping

    # ping.setup()

    # while True:
    #     ping.run()
    
    import spidev
    import time

    # Initialisation de l'interface SPI
    spi = spidev.SpiDev()  # Crée un objet SPI
    spi.open(0, 0)         # Bus SPI 0, périphérique 0 (MOSI: GPIO10, SCLK: GPIO11)
    spi.max_speed_hz = 500000  # Fréquence SPI (500 kHz)

    try:
        while True:
            # Envoie une séquence de données (ex. 0xAA puis 0x55)
            spi.xfer([0xAA])  # 10101010 en binaire
            time.sleep(0.1)
            spi.xfer([0x55])  # 01010101 en binaire
            time.sleep(0.1)

    finally:
        spi.close()  # Ferme l'interface SPI proprement

        
  
except Exception as e:
    print(f"Error: {e}")
    import os
    # restore from the backup
    import subprocess

    try:
        subprocess.run(['sudo', 'rm', '-rf', '/home/pi/Documents/PING2/*', '/home/pi/Documents/PING2/.*'], check=True)
        print("PING2 folder deleted")

    except subprocess.CalledProcessError as ee:
        print(f"fail to delete PING2 : {ee}")
    
    os.system(f'cp -r /home/pi/Documents/backup/. /home/pi/Documents/PING2/')
    print("PING2 folder restored from the backup")
    try:
        import pingpy.logger as logger
        logger.write_in_log("ERROR", __name__, "main", str(e))
    except:
        pass
    os.system(f'sleep 1 && python3 /home/pi/Documents/PING2/raspberry/src/main.py')
    exit(1) 