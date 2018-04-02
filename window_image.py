'''
AIM : create a window of NxN dimension, and slide it over the whole image
@author: Kushashwa Ravi Shrimali
'''
import cv2
import sys

def filter(region):
    rows = region.shape[0]
    cols = region.shape[1]
    print(rows, cols)
    for i in range(rows):
        for j in range(cols):
            # print(region[i][j])
            region[i][j] = [0, 0, 0]
    
    # return region
img = cv2.imread(sys.argv[1], 1) # read image - color mode
cv2.imshow(str(sys.argv[1]), img)
cv2.waitKey(0)
cv2.destroyAllWindows()

rows = img.shape[0]
cols = img.shape[1]

N = int(sys.argv[2])
print(int(cols/N))
i = 0
j = 0
while(True):
    # img[i:i+N] = filter(img[i:i+Ni,:, :])
    filter(img[i:i+N, j:j+N, :])
    # filter(img[:, j:j+N, :])
    i = i + N
    j = j + N
    

cv2.imshow(str(sys.argv[1]), img)
cv2.waitKey(0)
cv2.destroyAllWindows()
