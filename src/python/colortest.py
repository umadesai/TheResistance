import numpy as np
import cv2

image = cv2.imread("resistor-1-2k.jpg")

red = ([0, 0, 200], [75, 75, 255])
brown = ([0, 50, 160], [75, 110, 210]) #sort of

colors = [brown]

test1 = np.array([[1,2,3],[2,2,3]], dtype="uint8")
test2 = np.array([[1,2,4],[2,4,5]], dtype="uint8")
mask = np.array([1, 0, 0])

for lower, upper in colors:
    lower = np.array(lower, dtype = "uint8")
    upper = np.array(upper, dtype = "uint8")
    
    mask = cv2.inRange(image, lower, upper)
    output = cv2.bitwise_and(image, image, mask = mask)
 
    cv2.imshow("images", np.hstack([image, output]))
    cv2.waitKey(0)