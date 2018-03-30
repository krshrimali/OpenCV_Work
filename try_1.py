import cv2
import sys


def read_and_show(img):
    image = cv2.imread(img, 1) # reading in color mode
    
    # showing image now
    cv2.imshow(str(img), image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    return image

def show_webcam(mirror = False):
    cam = cv2.VideoCapture(0) # 0th ID let's say, change if another camera attached 
    while True:
        ret_val, frame = cam.read()
        # if ret_val is false, it means camera is not functioning

        if mirror:
            frame = cv2.flip('My Webcam', frame)

        cv2.imshow('My Webcam', frame)

        if cv2.waitKey(1) == 27:
            break

    cv2.destroyAllWindows()

def filter(img, original, threshold):
    # above threshold - white
    # below threshold - black
    
    dest_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    for j in range(dest_gray.shape[1]):
        for k in range(dest_gray.shape[0]):
            if(dest_gray[k][j] < threshold):
                print(dest_gray[k][j])
                dest_gray[k][j] = 255
            else:
                dest_gray[k][j] = 0

    # read_and_show(dest_gray)
    orig = cv2.imread(str(original), 1)
    cv2.imshow(str(threshold), dest_gray)
    cv2.imshow(str(original), orig)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    
def main():
    img = sys.argv[1]
    image = read_and_show(img) # reads and shows image at the same time. 
    # show_webcam() # mirroring is disabled as of now
    filter(image, img, 200)

if __name__ == '__main__':
    main()


