"""Cette version est pour référence, ce que le programme donne si on ne filtre pas les contours"""

import cv2
from PIL import Image
import matplotlib.pyplot as plt 
import numpy as np
import time

def saveIm(arr, name):
    im = Image.fromarray(arr)
    im.save("{}.jpeg".format(name))

def save(im, name):
    cv2.imwrite("{}.jpg".format(name), im)

img = cv2.imread("modele.png")
img2 = cv2.imread("modele.png")
height, width, trois = np.shape(img)
refCentre = (int(width/2), int(height/2))
aireImage = height*width


height, width, trois = np.shape(img)
edge_detected_image = cv2.Canny(img, 75, 200, L2gradient=True)
save(edge_detected_image, "Canny")
contours, hierarchy = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
cv2.drawContours(img, contours,  -1, (255,0,0), 2)
save(img, "Tous_contours")

contours_liste = []
contours, hierarchy = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
for contour in contours:
    approx = cv2.approxPolyDP(contour,0.01*cv2.arcLength(contour,True),True)
    area = cv2.contourArea(contour)
    if ((len(approx) > 10) & (area > 500) ):
        contours_liste.append(contour)
cv2.drawContours(img2, contours_liste,  -1, (255,0,0), 2)
(x,y),rayon = cv2.minEnclosingCircle(contours_liste[0])
distance = (width/2-x)
cv2.putText(img2, str(distance), (50,50), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2)
save(img2, "Unseul_contour")


cv2.imshow('Objects Detected',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
