try :
    from pingpy import ping
    import time
    ping.setup()
    while True:
        ping.run()
        time.sleep(0.01)

except Exception as e:
    print(f"Error: {e}")
    # print also the line where the error occured
    import traceback
    print(traceback.format_exc())

    import os
    # restore from the backup
    import subprocess
    import pingpy.logger as logger

    try:
        subprocess.run(['sudo', 'rm', '-rf', '/home/pi/Documents/PING2/*', '/home/pi/Documents/PING2/.*'], check=True)
        logger.write_in_log("INFO", __name__, "main", "PING2 folder deleted")

    except subprocess.CalledProcessError as e:
        logger.write_in_log("ERROR", __name__, "main", f'Error during deleting PING2 folder: {e}')
    
    try:
        os.system(f'cp -r /home/pi/Documents/backup/. /home/pi/Documents/PING2/')
        logger.write_in_log("INFO", __name__, "main", "PING2 folder restored from backup")    
    except subprocess.CalledProcessError as e:
        logger.write_in_log("ERROR", __name__, "main", f'Error during restoring PING2 folder from backup: {e}')

    try:
        subprocess.run(['python', '/home/pi/Documents/PING2/raspberry/src/main.py'], check=True)
        logger.write_in_log("INFO", __name__, "main", "Restarting app")    
    except subprocess.CalledProcessError as e:
        logger.write_in_log("ERROR", __name__, "main", f'Error during restarting app: {e}')
    exit(0)