import cv2 # pip import opencv-python pour l'installer, c'est la bibliothèque cruciale ici
# from PIL import Image # on ne s'en sert plus mais pas exclu qu'on en aie besoin si jamais l'image a besoin de contraste/luminosité
import numpy as np
import time

video = cv2.VideoCapture(0) # video est l'objet stockant le flux vidéo venant du téléphone

def make_480p(cap):
    cap.set(3, 640)
    cap.set(4, 480)

def make_720p(cap): 
    cap.set(3, 1280)
    cap.set(4, 720)

make_480p(video) # les deux fonctions réduisent la qualité vidéo (temps de traitement)

def traitement_image(img):
    height, width, trois = np.shape(img) # dimensions de l'image, en px
    #refCentre = (int(width/2), int(height/2))

    edge_detected_image = cv2.Canny(img, 75, 200, L2gradient=True) # détecte les contours et les affiche en noir et blanc sur une image binaire
    contours_liste = []
    contours, hierarchy = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE) # retourne une liste des contours
    for contour in contours: # filtrer les contours trouvés
        approx = cv2.approxPolyDP(contour,0.01*cv2.arcLength(contour,True),True) # transformer les contours en polygones
        aire = cv2.contourArea(contour)
        if ((len(approx) > 10) & (aire > 500) ): 
            # on ne garde que les poly à plus de 10 cotés (cercles) et avec une grande aire
            contours_liste.append(contour)
    cv2.drawContours(img, contours_liste,  -1, (255,0,0), 2) #trace les contours ayant survécu
    (x,y),rayon = cv2.minEnclosingCircle(contours_liste[0]) # repère les coordonnées du centre
    distance = (width/2-x) # distance selon x à l'origine pour l'asservissement après
    print('Contour: centre {},{}, rayon {}, distance : {}'.format(x,y,rayon, distance))
    cv2.putText(img, str(distance), (50,50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2) # affiche la distance sur l'image
    return (img, distance)

while True:
    _,frame = video.read() # capture l'image à un instant t de la vidéo
    try:
        image, dist = traitement_image(frame) 
    except: # le traitement a un temps d'exécution non négligeable, donc il faut cette clause pour éviter les erreurs
        image = frame
    cv2.imshow("RTSP",frame) # affiche l'image sur l'écran
    k = cv2.waitKey(1)
    if k == ord('q'): # appuyer sur q pour fermer l'image sur l'ordi
        break
    time.sleep(0.05) # délai adaptable, pas sûr qu'il soit nécessaire

video.release()
cv2.destroyAllWindows()
