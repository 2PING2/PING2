import os
import socket
import time

class StatusStreamer:
    def __init__(self, delay = 0.5, host = "0.0.0.0", port = 5356):
        self.sendToIP = self.lookForSshIp()
        self.server = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.server.bind((host, port))
        self.port = port
        self.lastTime = 0
    
    def lookForSshIp(self):
        result = os.popen("who").read()  # Exécute la commande 'who'
        for line in result.splitlines():
            parts = line.split()
            if len(parts) > 4 and parts[4].startswith("("):
                return parts[4][1:-1]  # Enlève les parenthèses autour de l'IP
        return None
    
    def sendStatus(self, input, t =  time.time()):
        if t - self.lastTime < 0.5:
            return
        self.lastTime = t
        if self.sendToIP is None:
            return
        
        playerPosition = [input.player[i].linearActuator.currentPose for i in range(4)]
        print("INFO"+ __name__+"sendStatus"+ "send new player position")

        # replace None by 0
        playerPosition = [0 if p is None else p for p in playerPosition]
        # Convertir en texte et envoyer
        data = ";".join([f"{p}" for p in playerPosition])
        self.server.sendto(data.encode(), (self.sendToIP, self.port))  # Remplace par l'IP de ton PC
            

