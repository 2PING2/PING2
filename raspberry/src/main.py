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
    GPIO.setup(PWM_PIN, GPIO.OUT)

    # Initialisation du PWM sur GPIO 10 avec une fréquence de 100 Hz
    pwm = GPIO.PWM(PWM_PIN, 100)  # Fréquence de 100 Hz
    pwm.start(0)  # Démarre avec un rapport cyclique de 0%
    try:
        while True:
            # Exemple : changer le rapport cyclique du PWM
            for duty_cycle in range(0, 101, 5):  # Augmente de 0% à 100%
                pwm.ChangeDutyCycle(duty_cycle)
                time.sleep(0.1)  # Pause de 100 ms
            for duty_cycle in range(100, -1, -5):  # Diminue de 100% à 0%
                pwm.ChangeDutyCycle(duty_cycle)
                time.sleep(0.1)
            
    except KeyboardInterrupt:
        print("Arrêt par l'utilisateur.")
    finally:
        # Nettoyage des GPIO
        pwm.stop()
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