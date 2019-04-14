import cv2
import sys # Arg Parsing, the easy way
import numpy as np

class Image:
    """
    Image Class
    ------------
    Parameters
    :img_path: (default=None), type: str
    -----------
    Methods
    * __len__(): args: None, returns: shape of the image
    * __str__(): args: None, returns: description of the image
    """
    def __doc__(self):
        usage = "Usage: img_object = Image(img_path='sample.png')"
        return usage

    def __init__(self, img_path=None):
        """
        Usage: img_object = Image(img_path='sample.png')
        """
        if img_path is None:
            return "img_path not mentioned"
        self.path = img_path
        self.img = cv2.imread(self.path, cv2.IMREAD_COLOR)
        self.gray = None

    def __len__(self):
        return self.img.shape[0] * self.img.shape[1] * self.img.shape[2]
    
    def __str__(self):
        shape = self.img.shape
        desc = "Image Shape: Width:  " + str(shape[0]) + " Height: " + \
                str(shape[1]) + ", Channels: " + str(shape[2])
        return desc
    
    def roi_selector(self):
        # Convert to Gray Scale
        self.gray = cv2.cvtColor(self.img, cv2.COLOR_BGR2GRAY)
        roi = cv2.selectROI(self.gray)
        return roi
    
    def hull(self):
        ret, thresh = cv2.threshold(self.gray, 200, 255, cv2.THRESH_BINARY)
        contours, hierarchy = cv2.findContours(thresh, cv2.RETR_TREE, \
                cv2.CHAIN_APPROX_SIMPLE)

        hull = []
        for i in range(len(contours)):
            hull.append(cv2.convexHull(contours[i], False))



    def blur(self):
        # Select ROI
        roi = self.roi_selector()
        croppedImage = self.gray[int(roi[1]):int(roi[1] + roi[3]), \
                int(roi[0]):int(roi[0] + roi[2])]
        blur = cv2.GaussianBlur(croppedImage, (13, 13), 0)
        
        empty = np.ones(self.gray.shape, self.gray.dtype)

        empty[int(roi[1]):int(roi[1] + roi[3]), \
                int(roi[0]):int(roi[0] + roi[2])] = blur
        
        cv2.imshow("Empty", empty)
        cv2.waitKey(0)
        cv2.destroyAllWindows()

        gray_copy = self.gray
        
        gray_copy[int(roi[1]):int(roi[1] + roi[3]), \
                int(roi[0]):int(roi[0] + roi[2])] = blur
        
        # res = cv2.bitwise_and(gray_copy, gray_copy, mask = empty)
        res = gray_copy
        return res    
img_obj = Image(sys.argv[1])
print(img_obj)
print(len(img_obj))
# TODO: img_obj = Image() should return an error
blur = img_obj.blur()
cv2.imshow("Blur", blur)
cv2.waitKey(0)
cv2.destroyAllWindows()
