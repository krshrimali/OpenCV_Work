import cv2
from matplotlib import pyplot as plt
import numpy as np
import sys

path = '/home/krshrimali/Pictures/'

def help():
    print("----------------------------------------------------------")
    print("Parameters: ")
    print("1.   image paths")
    print("----------------------------------------------------------")
    print("Result will be histogram in R, G, B Channel of the image. ")
    
    print("Author: Kushashwa Ravi Shrimali")


def show(img, path=1):
    '''
    function to show image read by the user
    parameter: image path or image read and path variable
    path = 1 if path given
    path = 0 if read image given
    returns image read by the user
    '''
    if(path):
        img2 = cv2.imread(img)
        cv2.imshow(str(img), img2)
    else:
        cv2.imshow("Image", img)

    cv2.waitKey(0)
    cv2.destroyAllWindows()

    if(path):
        return img2
    else:
        return img

def draw_hist(img):
    print("You want histograms of R, G, B Channels in one image or separate?")
    decide = input("separate or single? sep/sing ")
    flag = []
    color = ('b', 'g', 'r')
    for i, col in enumerate(color):
        hist = cv2.calcHist([img], [i], None, [256], [0,256])
        plt.plot(hist, color = col)
        plt.xlim([0, 256])
        if(decide == "sep"):
            plt.show()
            flag.append(1)
    if(len(flag) != 3):
        plt.show()

# number of image paths entered
argc = len(sys.argv) - 1

if(argc == 0):
    print("You have to enter image paths in order to view their histograms")
    help()
    sys.exit(0)

print("Number of images: " + str(argc))

# display histograms of all the images entered
# for R, G, B Channels
for i in range(argc):    
    print(sys.argv[i+1])
    img_path = path + sys.argv[i+1]
    img = show(img_path)
    draw_hist(img)
