import socket

HOST = "0.0.0.0"  # Écoute sur toutes les interfaces réseau
PORT = 5356       # Doit correspondre au port utilisé par la Raspberry Pi

def receive_positions():
    client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client.bind((HOST, PORT))
    print(f"🎯 En attente des positions sur {HOST}:{PORT}...")

    while True:
        data, addr = client.recvfrom(1024)  # Attente des données (1024 octets max)
        positions = data.decode().split(";")  # Décodage des positions reçues
        positions = [int(p) for p in positions]  # Conversion en int
        print(f"📡 Positions reçues depuis {addr[0]} : {positions}")

if __name__ == "__main__":
    receive_positions()
