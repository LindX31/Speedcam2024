import cv2
from PIL import Image
import matplotlib.pyplot as plt 
import numpy as np
import time

video = cv2.VideoCapture(0)

def make_480p(cap):
    cap.set(3, 640)
    cap.set(4, 480)

make_480p(video)

def traitement_image(img):
    height, width, trois = np.shape(img)
    #refCentre = (int(width/2), int(height/2))

    edge_detected_image = cv2.Canny(img, 75, 200, L2gradient=True)
    contours_liste = []
    contours, hierarchy = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    for contour in contours:
        approx = cv2.approxPolyDP(contour,0.01*cv2.arcLength(contour,True),True)
        aire = cv2.contourArea(contour)
        if ((len(approx) > 10) & (aire > 100) ):
            contours_liste.append(contour)
    cv2.drawContours(img, contours_liste,  -1, (255,0,0), 2)
    (x,y),rayon = cv2.minEnclosingCircle(contours_liste[0])
    distance = (width/2-x)
    print('Contour: centre {},{}, rayon {}, distance : {}'.format(x,y,rayon, distance))
    cv2.putText(img, str(distance), (50,50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
    return (img, distance)

while True:
    _,frame = video.read()
    try:
        image, dist = traitement_image(frame)
    except:
        image = frame
    cv2.imshow("RTSP",frame)
    k = cv2.waitKey(1)
    if k == ord('q'):
        break
    time.sleep(0.05)

video.release()
cv2.destroyAllWindows()
