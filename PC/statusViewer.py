import socket
import numpy as np
import cv2
import math
import threading

HOST = "0.0.0.0"  # Écoute sur toutes les interfaces réseau
PORT = 5356       # Doit correspondre au port utilisé par la Raspberry Pi

# Liste pour stocker les positions des joueurs
positions = [0, 0, 0, 0]  # Initialisation avec des positions fictives pour 4 joueurs

# Taille de l'image (dimensions du terrain de jeu)
WIDTH, HEIGHT = 800, 800

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
        player_positions.append((x + WIDTH // 2, y + HEIGHT // 2))  # Centrer sur l'écran
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
        
        # Mise à jour de l'affichage
        update_display()

def update_display():
    # Conversion des positions en coordonnées x, y
    player_positions = map_positions_to_coordinates()
    
    # Création d'une image vide (fond noir)
    img = np.zeros((HEIGHT, WIDTH, 3), dtype=np.uint8)
    
    # Dessin du carré représentant les limites du terrain de jeu
    cv2.rectangle(img, (100, 100), (WIDTH - 100, HEIGHT - 100), (255, 255, 255), 2)  # Carré blanc pour les limites
    
    # Dessin des cercles pour la position des joueurs
    for i, (x, y) in enumerate(player_positions):
        cv2.circle(img, (int(x), int(y)), 10, (0, 0, 255), -1)  # Cercle rouge pour chaque joueur
    
    # Affichage de l'image
    cv2.imshow('Positions des joueurs', img)

    # Attente d'une touche pour fermer la fenêtre
    cv2.waitKey(1)

# Lancer le thread de réception des positions en arrière-plan
thread = threading.Thread(target=receive_positions)
thread.daemon = True
thread.start()

# Boucle principale pour afficher les positions en temps réel
while True:
    pass  # Le thread de réception gère la mise à jour en temps réel
