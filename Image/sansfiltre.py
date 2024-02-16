"""Cette version est pour référence, ce que le programme donne si on ne filtre pas les contours"""

import cv2
from PIL import Image
import matplotlib.pyplot as plt 
import numpy as np
import time

video = cv2.VideoCapture(0)

def traitement_image(img):
    height, width, trois = np.shape(img)
    #refCentre = (int(width/2), int(height/2))

    edge_detected_image = cv2.Canny(img, 75, 200, L2gradient=True)
    contours, hierarchy = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    cv2.drawContours(img, contours,  -1, (255,0,0), 2)
    (x,y),rayon = cv2.minEnclosingCircle(contours[0])
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
    time.sleep(0.1)

video.release()
cv2.destroyAllWindows()
