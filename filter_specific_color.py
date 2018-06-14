'''
@author: Kushashwa Ravi Shrimali
Program to filter out particular color from the image.

On way to make a drawing tool, with hands.

Arguments: <image_sample path> <threshold value> 
[Note: currently it takes green channel sample, will be changed soon. On experimental basis]

Usage: python3 try_1.py sample2.jpg 240 
[example] - image uploaded on GitHub repository

To Do: A lot ;) 
'''
import cv2
import sys


def read_and_show(img):
    '''
    function: shows and returns an image
    argument: <image path>
    '''
    image = cv2.imread(img, 1) # reading in color mode
    
    # showing image now
    cv2.imshow(str(img), image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    return image

def show_webcam(threshold, filter_ = 0, mirror = False):
    '''
    function: shows filtered output image, doesn't show original image as of now - to save cost and efficiency

    argument: <threshold>(int) <filter_ = 0>(int) <mirror value>(bool)

    usage: show_webcam(240, 1) 
    [Example]
    '''
    cam = cv2.VideoCapture(0) # 0th ID let's say, change if another camera attached 
    while True:
        ret_val, frame = cam.read()
        # if ret_val is false, it means camera is not functioning

        if mirror:
            frame = cv2.flip('My Webcam', frame)
        
        if(filter_ == 1):
            frame_ = filter(frame, threshold)

            cv2.imshow('Filtered', frame_)

            if cv2.waitKey(1) == 27:
                break
        else:
            cv2.imshow('My Webcam', frame)
        
            if cv2.waitKey(1) == 27:
                break
        
    cv2.destroyAllWindows()

def filter(dest_gray, threshold):
    '''
    function:
        above threshold - white
        below threshold - black

    argument: <image> <threshold>(int)

    usage: filter(image, 240)

    return value: returns filtered frame
    '''
    # dest_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    list_ = []

    '''
    for j in range(dest_gray.shape[1]):
        for k in range(dest_gray.shape[0]):
            list_.append(dest_gray[k][j])
    
    print(set(list_))
    '''

    for j in range(dest_gray.shape[1]):
        for k in range(dest_gray.shape[0]):
            if(dest_gray[k][j][1] < threshold):
                # print(dest_gray[k][j])
                dest_gray[k][j][0] = 0
                dest_gray[k][j][2] = 0
                dest_gray[k][j][1] = 0
            else:
                dest_gray[k][j][0] = 0
                dest_gray[k][j][2] = 0
                dest_gray[k][j][1] = 255

    # read_and_show(dest_gray)
    # orig = cv2.imread(str(original), 1)
    return dest_gray 

def main():
    img = sys.argv[1]
    threshold = int(sys.argv[2])
    image = read_and_show(img) # reads and shows image at the same time. 
    show_webcam(threshold, 1) # mirroring is disabled as of now
    # filter(image, img, threshold)
    
if __name__ == '__main__':
    main()
