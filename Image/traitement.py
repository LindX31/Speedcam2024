import cv2
from PIL import Image
import easygui
import matplotlib.pyplot as plt 
import numpy as np

myimg = Image.open("modele.png")
#myimg.show()
img = np.array(myimg)

print(img[0, 0])

# REDIMENSIONNER L'IMAGE POUR REDUIRE LA COMPLEXITE
def redim_img(img,coeff=3) :
    colonnes = list(range(0,img.shape[1]))
    colonnes = colonnes[0::coeff]
    lignes = list(range(0,img.shape[0]))
    lignes = lignes[0::coeff] 
    if len(img.shape)==3 :
        img_temp = img[lignes,:,:]
        img_temp = img_temp[:,colonnes,:]
    else :
        img_temp = img[lignes,:]
        img_temp = img_temp[:,colonnes]
    
    return(img_temp)

#img = redim_img(img, 50)



im = cv2.imread('ideal.png')
height, width, trois = np.shape(im)
refCentre = (int(width/2), int(height/2))
aireImage = height*width

bilateral_filtered_image = cv2.bilateralFilter(im, 5, 175, 175)
edge_detected_image = cv2.Canny(bilateral_filtered_image, 75, 200)
cv2.imshow('Edge', edge_detected_image)
cv2.waitKey(0)
cv2.destroyAllWindows()
contour_list = []
contours_bis, hierarchy = cv2.findContours(edge_detected_image, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
for contour in contours_bis:
    approx = cv2.approxPolyDP(contour,0.01*cv2.arcLength(contour,True),True)
    area = cv2.contourArea(contour)
    if ((len(approx) > 15) & (area > 300) ):
        contour_list.append(contour)
cv2.drawContours(im, contour_list,  -1, (255,0,0), 2)
(x,y),radius = cv2.minEnclosingCircle(contour_list[0])
print('Contour: centre {},{}, radius {}'.format(x,y,radius))
cv2.imshow('Objects Detected',im)
cv2.waitKey(0)
cv2.destroyAllWindows()


# Convert to grayscale and threshold
imgray = cv2.cvtColor(im,cv2.COLOR_BGR2GRAY)
ret,thresh = cv2.threshold(imgray,127,255,cv2.THRESH_BINARY_INV)
#ret,thresh = cv2.threshold(im,127,255,0)
# Find contours, draw on image and save
contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
outline = []
for cnt in contours:
    area = cv2.contourArea(cnt)
    (x,y),radius = cv2.minEnclosingCircle(cnt)
    if area > 100000 and radius >100:
        outline.append(cnt)
cv2.drawContours(im, outline, -1, (0,255,0), 3)

#im[refCentre[1], refCentre[0]] = [255, 0, 0]
cv2.imwrite('result.png',im)

# Show user what we found
"""
for cnt in outline:
    (x,y),radius = cv2.minEnclosingCircle(cnt)
    area = cv2.contourArea(cnt)
    if radius > 100:
        center = (int(x),int(y))
        radius = int(radius)
        distSq = (refCentre[0]-center[0])**2 + (refCentre[1]-center[1])**2
        print('Contour: centre {},{}, radius {}, area {} distance au centre {}'.format(x,y,radius, area, distSq))
"""

cv2.imshow('im', im)
cv2.waitKey(0)
cv2.imshow('thresh', thresh)
cv2.waitKey(0)
cv2.destroyAllWindows()


#SOLUTION EN COMPLEXITE QUADRATIQUE A PROSCRIRE
"""
for i in range (height):
    for j in range (width):
        if img[i][j][0] <100 and img[i][j][1] <100 and img[i][j][2] < 100 : 
            img[i][j] = np.zeros(3)
        else:
            img[i][j] = np.array([255, 255, 255])
"""


#plt.imshow(imgray)
#plt.show()