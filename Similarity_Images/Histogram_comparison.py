# Resource: https://docs.opencv.org/3.1.0/d1/db7/tutorial_py_histogram_begins.html

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

def draw_hist(imgPath, img, full = None):
    print("Drawing Histogram")
    print("You want histograms of R, G, B Channels in one image or separate?")
    decide = int(input("separate or single? 0/1 "))
    flag = 0
    color = ('b', 'g', 'r')
    for i, col in enumerate(color):
        hist = cv2.calcHist([img], [i], full, [256], [0,256])
        plt.plot(hist, color = col)
        plt.xlim([0, 256])
        if(decide == 0):
            plt.show()
            plt.savefig(imgPath + '_' + col + '.png')
            flag += 1
            print(flag)
    if(flag != 3):
        plt.show()
        plt.savefig(imgPath + '.png')

# number of image paths entered
argc = len(sys.argv) - 1

def create_mask(img):
    mask = np.zeros(img.shape[:2], np.uint8)
    mask[100:300, 100:400] = 255
    masked_img = cv2.bitwise_and(img, img, mask = mask)
    show(masked_img, 0)
    draw_hist(img, mask)

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
    
    print("Reading image: " + img_path)
    
    img = show(img_path)
    draw_hist(img_path, img)
    
    # create_mask(img)
