import cv2

#video = cv2.VideoCapture("rtsp://192.168.1.190/video")
video = cv2.VideoCapture(0)
# cv2.namedWindow('displaymywindows', cv2.WINDOW_NORMAL)

while True:
	_,frame = video.read()
	cv2.imshow("RTSP",frame)
	k = cv2.waitKey(1)
	if k == ord('q'):
		break

video.release()
cv2.destroyAllWindows()
