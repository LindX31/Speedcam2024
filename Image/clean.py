import cv2
from PIL import Image
import matplotlib.pyplot as plt 
import numpy as np

img = cv2.imread("japan.png")
height, width, trois = np.shape(img)
refCentre = (int(width/2), int(height/2))
aireImage = height*width

edge_detected_image = cv2.Canny(img, 75, 200, L2gradient=True)
cv2.imshow('Objects Detected',edge_detected_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
contours_liste = []
contours, hierarchy = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
for contour in contours:
    approx = cv2.approxPolyDP(contour,0.01*cv2.arcLength(contour,True),True)
    area = cv2.contourArea(contour)
    if ((len(approx) > 15) & (area > 100) ):
        contours_liste.append(contour)
cv2.drawContours(img, contours_liste,  -1, (255,0,0), 2)
(x,y),radius = cv2.minEnclosingCircle(contours_liste[0])
print('Contour: centre {},{}, radius {}'.format(x,y,radius))
cv2.imshow('Objects Detected',img)
cv2.waitKey(0)
cv2.destroyAllWindows()