/*
 Using OpenCV Transparent API, to make execution faster.
 Use only if advanced and complex functions are used, else
 over head of moving image to GPU dominates timing.
 [Reference: learnopencv.com/opencv-transparent-api/]

Usage:
g++ filename.cpp `pkg-config --libs opencv --cflags`

Functions:
Applys gaussian filter and edge filter detectors.

Credits:
learopencv and Kushashwa Ravi Shrimali
 */
#include <opencv2/opencv.hpp>
#include <iostream>

// to verify data types of image objects
#include <typeinfo>

using namespace cv;
using namespace std;

// functions to convert from Mat to UMat
UMat convert_mat_UMat(Mat);
UMat convert_mat_UMat_copy(Mat);

int main(int argc, char** argv) {
    // create image objects
    // OpenCV Transparent API way - UMat matrix used.
    // Options :
    // Convert Mat to UMat
    Mat img, gray, edge_filtered,  gaussian_filtered;
    
    // read the image in color mode
    // copy the read image to img, OpenCV Transparent API way
    imread(argv[1], IMREAD_COLOR).copyTo(img);
    
    UMat img2 = convert_mat_UMat(img);
    cout << typeid(img2).name() << endl;


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

// flags - ?
UMat convert_mat_UMat(Mat img) {
    return img.getUMat(ACCESS_FAST);
}

Umat convert_mat_UMat_copy(Mat img) {
    UMat umat;
    mat.copyTo(umat);
    return umat;
}


