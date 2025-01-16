try :
    # from pingpy import ping

    # ping.setup()

    # while True:
    #     ping.run()
    
    import RPi.GPIO as GPIO
    import time

    # Configuration du GPIO
    GPIO.setmode(GPIO.BCM)  # Utilise la numérotation BCM
    PWM_PIN = 10
    #make sure GPIO is not already in use
    GPIO.setup(PWM_PIN, GPIO.OUT)

    try:
        while True:
            # Exemple : changer l'état de la LED
            GPIO.output(PWM_PIN, GPIO.HIGH)
            time.sleep(1)
            GPIO.output(PWM_PIN, GPIO.LOW)
            time.sleep(1)
            
    except KeyboardInterrupt:
        print("Arrêt par l'utilisateur.")
    finally:
        # Nettoyage des GPIO
        GPIO.cleanup()

        
  
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