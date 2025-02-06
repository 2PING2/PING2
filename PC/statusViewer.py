import socket
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

HOST = "0.0.0.0"  # Écoute sur toutes les interfaces réseau
PORT = 5356       # Doit correspondre au port utilisé par la Raspberry Pi

# Liste pour stocker les positions des joueurs (x, y)
positions = [(0, 0)] * 4  # Initialisation avec des positions fictives (4 joueurs)

def receive_positions():
    client = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    client.bind((HOST, PORT))
    print(f"🎯 En attente des positions sur {HOST}:{PORT}...")

    while True:
        data, addr = client.recvfrom(1024)  # Attente des données (1024 octets max)
        positions_data = data.decode().split(";")  # Décodage des positions reçues
        positions_data = [float(p) for p in positions_data]  # Conversion en float
        
        # Met à jour les positions
        global positions
        positions = [(positions_data[i], positions_data[i+1]) for i in range(0, len(positions_data), 2)]
        
        # Mise à jour du graphique
        update_graph()

def update_graph():
    # Mise à jour de la position sur le graphique
    x_vals, y_vals = zip(*positions)  # Séparer les positions en x et y
    scatter.set_offsets(list(zip(x_vals, y_vals)))  # Mettre à jour les coordonnées des joueurs

def animate(i):
    pass  # Cette fonction est utilisée par FuncAnimation, mais aucune action ici

# Création du graphique
fig, ax = plt.subplots()
ax.set_xlim(-10, 10)  # Limites du graphique, ajuster selon tes données
ax.set_ylim(-10, 10)
scatter = ax.scatter([p[0] for p in positions], [p[1] for p in positions], c='red')

# Animation pour mettre à jour le graphique en temps réel
ani = FuncAnimation(fig, animate, interval=100)  # Rafraîchissement toutes les 100ms

# Lancer le thread de réception des positions en arrière-plan
import threading
thread = threading.Thread(target=receive_positions)
thread.daemon = True
thread.start()

# Affichage du graphique
plt.show()