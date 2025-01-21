try :
    # from pingpy import ping
    import time

    ping.setup()


    while True:
        ping.run()
        time.sleep(0.01)
    

  
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
    os.system(f'sleep 0.1 && python3 /home/pi/Documents/PING2/raspberry/src/main.py')
    exit(1)