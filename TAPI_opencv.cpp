#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;
int main(int argc, char** argv) {
    // create image objects
    Mat img, gray, edge_filtered,  gaussian_filtered;
    
    // read the image in color mode
    img = imread(argv[1], IMREAD_COLOR);

    // resize the image, optional, if higher resolution
    // convert image to grayscale, store into gray object
    cvtColor(img, gray, COLOR_BGR2GRAY);

    // apply gaussian blur filter
    GaussianBlur(gray, gaussian_filtered, Size(7, 7), 1.5);

    // apply canny edge detection
    Canny(gaussian_filtered, edge_filtered, 0, 100);
    
    imshow("edge_filtered", edge_filtered);
    imshow("gaussian_filtered", gaussian_filtered);
    imshow("img", img);
    imshow("gray", gray);
    
    imwrite("./images/edge_filtered_2.jpg", edge_filtered);
    waitKey(0);
    
}


