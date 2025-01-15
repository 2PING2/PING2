# try :
from pingpy import ping

ping.setup()

while True:
    ping.run()
        

# except Exception as e:
#     import os
#     # restore the backup
#     os.system(f'cp -r /home/pi/Documents/backup/* /home/pi/Documents/PING2')
#     print("Error in main.py: ", e)
#     try:
#         import pingpy.logger as logger
#         logger.write_in_log("ERROR", __name__, "main", str(e))
#     except:
#         pass
#     os.system(f'sleep 1 && python3 /home/pi/Documents/PING2/raspberry/src/main.py')
#     exit(1) 