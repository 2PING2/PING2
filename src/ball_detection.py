import cv2
import numpy as np
import json
from datetime import datetime as Date

# Initialisation du soustracteur de fond
bg_subtractor = cv2.createBackgroundSubtractorMOG2(history=500, varThreshold=16, detectShadows=True)

# Déclaration de variables globales
roi1 = None
ball_color = None
ROI1_mask = None
# ajout dans le fichier log avec date, type de message et message et le fichier d'où vient l'erreur
def log(message, type):
    with open("log_vision.txt", "a") as file:
        file.write(f"{Date.now()} - {type} : {message}\n")

# Redimensionne la frame pour l'affichage
def resize_for_display(frame, max_display_size=800):
    height, width = frame.shape[:2]
    if height > width:
        new_height = min(height, max_display_size)
        new_width = int(width * new_height / height)
    else:
        new_width = min(width, max_display_size)
        new_height = int(height * new_width / width)
    return cv2.resize(frame, (new_width, new_height))

# Sélection de la ROI
def select_roi(frame, max_display_size=800):
    global roi1, ROI1_mask
    resized = resize_for_display(frame, max_display_size)
    resize_factor = frame.shape[1] / resized.shape[1]
    roi1 = cv2.selectROI("Sélectionner la ROI", resized)
    roi1 = tuple(int(x * resize_factor) for x in roi1)
    with open("roi.json", "w") as file:
        json.dump(roi1, file)
    x, y, w, h = roi1
    ROI1_mask = np.zeros_like(frame)
    ROI1_mask[y:y+h, x:x+w] = 1

# Sélection de la couleur de la balle
def select_color(frame):
    global ball_color
    ball_roi = cv2.selectROI("Sélectionner la couleur de la balle", frame)
    larger_roi = frame[ball_roi[1]:ball_roi[1]+ball_roi[3], ball_roi[0]:ball_roi[0]+ball_roi[2]]
    
    average_color_bgr = cv2.mean(larger_roi)[:3]
    ball_color_bgr = np.array([[average_color_bgr]], dtype=np.uint8)
    ball_color_hsv = cv2.cvtColor(ball_color_bgr, cv2.COLOR_BGR2HSV)

    color_name = input("Entrez le nom de cette couleur de balle : ")

    with open("color.json", "r") as file:
        color_data = json.load(file) if file else {}
    color_data[color_name] = ball_color_hsv[0][0].tolist()
    with open("color.json", "w") as file:
        json.dump(color_data, file)
    ball_color = np.array([[ball_color_hsv[0][0]]], dtype=np.uint8)

# Charger une couleur de balle par son nom
def load_ball_color():
    global ball_color
    try:
        with open("color.json", "r") as file:
            colors_data = json.load(file)
        
        color_name = input(f"Entrez le nom de la couleur à charger parmi : {', '.join(colors_data.keys())} : ")

        if color_name in colors_data:
            ball_color = np.array([[colors_data[color_name]]], dtype=np.uint8)
            print(f"Couleur chargée pour : {color_name}")
        else:
            print("Nom de couleur non trouvé.")
            log("Nom de couleur non trouvé.", "Erreur")
    except FileNotFoundError:
        print("Fichier color.json introuvable. Veuillez d'abord sauvegarder une couleur avec la touche c.")
        log("Fichier color.json introuvable. Veuillez d'abord sauvegarder une couleur avec la touche c.", "Erreur")

# Application de la ROI
def apply_roi(frame):
    if roi1 is not None:
        x, y, w, h = roi1
        return frame[y:y+h, x:x+w]
    return frame

# Masque de couleur pour isoler la balle
def apply_color_mask(frame, tolerance=40):
    global ball_color
    if ball_color is None:
        return np.zeros(frame.shape[:2], dtype=np.uint8)

    # Convertir la frame en HSV
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    
    # Extraire la couleur de la balle en HSV
    lower = np.array([ball_color[0][0][0] - tolerance, max(ball_color[0][0][1] - tolerance, 0), max(ball_color[0][0][2] - tolerance, 0)])
    upper = np.array([ball_color[0][0][0] + tolerance, min(ball_color[0][0][1] + tolerance, 255), min(ball_color[0][0][2] + tolerance, 255)])

    # Créer le masque de couleur
    return cv2.inRange(hsv_frame, lower, upper)

# Soustraction de fond
def background_subtraction(frame, remove_shadow=True):
    mask = bg_subtractor.apply(frame)
    if remove_shadow:
        _, mask = cv2.threshold(mask, 127, 255, cv2.THRESH_BINARY)
    return mask

# Fonction principale pour détecter la balle
def detect_ball(roi):
    global combined_mask
    # Soustraction de fond
    motion_mask = background_subtraction(roi, remove_shadow=True)

    # Masque de couleur
    color_mask = apply_color_mask(roi) if ball_color is not None else np.zeros_like(motion_mask)
    
    # Combinaison des masques
    combined_mask = cv2.bitwise_and(motion_mask, color_mask)
    combined_mask = cv2.GaussianBlur(combined_mask, (5, 5), 0)

    # Détection de cercles (forme circulaire)
    circles = cv2.HoughCircles(combined_mask, cv2.HOUGH_GRADIENT, dp=1.2, minDist=100,
                               param1=30, param2=30, minRadius=20, maxRadius=150)
    if circles is not None:
        x, y, radius = circles[0][0].astype(int)
        return x, y, radius
    return None

# Chargement de la vidéo
video_path = 'config/video/video_camera_2.0.mp4'
#video_path_2 = 'config/video/video_camera_2.0.mp4'
cap1 = cv2.VideoCapture(video_path)
#cap2 = cv2.VideoCapture(video_path_2)


if not cap1.isOpened():
    print("Erreur : Impossible de lire la vidéo.")
    log("Impossible de lire la video.", "Erreur")
    exit()

""" if not cap1.isOpened():
    print("Erreur : Impossible de lire la vidéo.")
    log("Impossible de lire la video.", "Erreur")
    exit() """


""" #Récupérer les propriétés de la vidéo
frame_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
fps = cap.get(cv2.CAP_PROP_FPS)

# # Définir le codec et initialiser le VideoWriter pour sauvegarder la vidéo avec les détections
output_path = 'video_apres_detection/video_detection_balle_bleue.mp4'
fourcc = cv2.VideoWriter_fourcc(*'mp4v')  # Codec pour MP4
out = cv2.VideoWriter(output_path, fourcc, fps, (frame_width, frame_height)) """


# Chargement de la ROI si disponible
try:
    with open("roi.json", "r") as file:
        roi1 = tuple(json.load(file))
except FileNotFoundError:
    print("Sélection de la ROI nécessaire.")
    log("Sélection de la ROI nécessaire.", "Erreur")

# Boucle principale
while True:
    ret1, frame1 = cap1.read()
    #ret2, frame2 = cap2.read()
    if not ret1:
        break

    if roi1 is None:
        select_roi(frame1)
        #select_roi(frame2)
    roi_frame_1 = apply_roi(frame1)
    #roi_frame_2 = apply_roi(frame2)


    if ball_color is not None and roi1 is not None:
        ball_position_1 = detect_ball(roi_frame_1)
        #ball_position_2 = detect_ball(roi_frame_2)
        if ball_position_1:
            x, y, radius = ball_position_1
            cv2.circle(frame1, (x + roi1[0], y + roi1[1]), radius, (0, 255, 0), 2)
            cv2.circle(frame1, (x + roi1[0], y + roi1[1]), 2, (0, 0, 255), 3)
            print(f"Balle détectée aux coordonnées : ({x + roi1[0]}, {y + roi1[1]}), Rayon : {radius}")
        else:
            print("Balle non détectée.")

        """ if ball_position_2:
            x, y, radius = ball_position_2
            cv2.circle(frame2, (x + roi1[0], y + roi1[1]), radius, (0, 255, 0), 2)
            cv2.circle(frame2, (x + roi1[0], y + roi1[1]), 2, (0, 0, 255), 3)
            print(f"Balle détectée aux coordonnées : ({x + roi1[0]}, {y + roi1[1]}), Rayon : {radius}") """
        
        cv2.imshow("Masque de mouvement", resize_for_display(background_subtraction(roi_frame_1)))
        cv2.imshow("Masque de couleur", resize_for_display(apply_color_mask(roi_frame_1)))
        cv2.imshow("Masque combine", resize_for_display(combined_mask))
        cv2.imshow("roi", roi_frame_1)

    else:
        print("Sélection de la couleur de la balle nécessaire. Appuyez sur 'c' pour sélectionner la couleur.")
    
    #Sauvegarde de la vidéo avec les détections
    #out.write(frame)
    cv2.imshow("Flux vidéo", resize_for_display(frame1))
    #cv2.imshow("Flux vidéo", resize_for_display(frame2))


    # Contrôles
    key = cv2.waitKey(10)
 
    if key == ord('c'):
        select_color(roi_frame_1)
    if key == ord('l'):  # 'l' pour charger une couleur spécifique
        load_ball_color()
    if key == ord('q'):
        break

    cv2.waitKey(1)

cap1.release()
#cap2.release()
#out.release()
cv2.destroyAllWindows()

# print(f"Vidéo enregistrée sous {output_path}")