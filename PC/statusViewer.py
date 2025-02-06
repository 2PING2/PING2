import socket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import math
import threading

HOST = "0.0.0.0"  # Écoute sur toutes les interfaces réseau
PORT = 5356       # Doit correspondre au port utilisé par la Raspberry Pi

# Liste pour stocker les positions des joueurs
positions = [0, 0, 0, 0]  # Initialisation avec des positions fictives pour 4 joueurs

# Fonction de calcul des positions 2D
def compute2DPosition(linearActuatorPose, playerId):
    theta = playerId * 90  # Angle de 90° par joueur
    
    x = linearActuatorPose
    y = 300  # Valeur d'offset, à ajuster si nécessaire
    
    # Calcul des coordonnées après rotation
    xbis = x * math.cos(math.radians(theta)) - y * math.sin(math.radians(theta))
    ybis = x * math.sin(math.radians(theta)) + y * math.cos(math.radians(theta))
    
    return (xbis, ybis)

# Conversion des valeurs reçues en coordonnées x, y
def map_positions_to_coordinates():
    player_positions = []
    for playerId, linearActuatorPose in enumerate(positions):
        x, y = compute2DPosition(linearActuatorPose, playerId)
        player_positions.append((x, y))
    return player_positions

def receive_positions():
    client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client.bind((HOST, PORT))
    print(f"🎯 En attente des positions sur {HOST}:{PORT}...")

    while True:
        data, addr = client.recvfrom(1024)  # Attente des données (1024 octets max)
        positions_data = data.decode().split(";")  # Décodage des positions reçues
        positions_data = [float(p) for p in positions_data]  # Conversion en float
        
        # print(f"🎯 Positions reçues: {positions_data}")
        
        # Met à jour les positions
        global positions
        positions = positions_data  # On remplace les anciennes positions par les nouvelles
        
        # Mise à jour du graphique
        update_graph()

def update_graph():
    # Conversion des positions en coordonnées x, y
    player_positions = map_positions_to_coordinates()
    
    # Mise à jour des positions sur le graphique
    # print(f"🎯 Mise à jour des positions: {player_positions}")
    scatter.set_offsets(player_positions)  # Mettre à jour les coordonnées des joueurs

def animate(i):
    pass  # Cette fonction est utilisée par FuncAnimation, mais aucune action ici

# Création du graphique
fig, ax = plt.subplots()
ax.set_xlim(-400, 400)  # Limites du graphique pour un carré de 400mm
ax.set_ylim(-400, 400)
scatter = ax.scatter([0, 0, 0, 0], [0, 0, 0, 0], c='red')  # Initialisation avec des positions fictives

# Animation pour mettre à jour le graphique en temps réel
ani = FuncAnimation(fig, animate, interval=100)  # Rafraîchissement toutes les 100ms

# Lancer le thread de réception des positions en arrière-plan
thread = threading.Thread(target=receive_positions)
thread.daemon = True
thread.start()

# Affichage du graphique
plt.show()
