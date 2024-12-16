""" code pour sauvegarder le flux vidéo de deux caméras en mp4 """

import cv2
import numpy as np



cap1 = cv2.VideoCapture(1)
cap2 = cv2.VideoCapture(2)

if not cap1.isOpened() or not cap2.isOpened():
    print("Erreur : Impossible de lire les caméras.")
    exit()

# initialiser les codecs pour l'enregistrement
fourcc = cv2.VideoWriter_fourcc(*'mp4v')
fps1 = cap1.get(cv2.CAP_PROP_FPS)
fps2 = cap2.get(cv2.CAP_PROP_FPS)
frame_width = int(cap1.get(cv2.CAP_PROP_FRAME_WIDTH))
frame_height = int(cap1.get(cv2.CAP_PROP_FRAME_HEIGHT))
output_path = 'video/video_camera_1.0.mp4'
out1 = cv2.VideoWriter(output_path, fourcc, fps1, (frame_width, frame_height))
output_path = 'video/video_camera_2.0.mp4'
out2 = cv2.VideoWriter(output_path, fourcc, fps2, (frame_width, frame_height))


while True:
    ret1, frame1 = cap1.read()
    ret2, frame2 = cap2.read()

    if not ret1 or not ret2:
        print("Erreur : Impossible de lire les images.")
        break

    out1.write(frame1)
    out2.write(frame2)

    key = cv2.waitKey(1) & 0xFF
    if key == ord('q'):
        break


out1.release()
out2.release()
cap1.release()
cap2.release()
cv2.destroyAllWindows()
    
