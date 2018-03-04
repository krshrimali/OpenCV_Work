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
// for recording time
#include <sys/time.h>

// to verify data types of image objects
// #include <typeinfo>

using namespace cv;
using namespace std;

// functions to convert from Mat to UMat
UMat convert_mat_UMat(Mat);
UMat convert_mat_UMat_copy(Mat);

// linux alternate to getTickCount()
/*unsigned getTickCount() {
    struct timeval tv;
    if(gettimeofday(&tv, NULL) != 0) 
        return 0;
    return (tv.tv_sec * 1000) + (tv.tv_sec/1000);
}*/

int main(int argc, char** argv) {
    // create image objects
    // OpenCV Transparent API way - UMat matrix used.
    // Options :
    // Convert Mat to UMat
    double startT = (double)getTickCount();
    cout << startT/getTickFrequency() << endl;
    Mat img, gray, edge_filtered,  gaussian_filtered;
    
    // read the image in color mode
    // copy the read image to img, OpenCV Transparent API way
    imread(argv[1], IMREAD_COLOR).copyTo(img); 
    // UMat img2 = convert_mat_UMat(img);
    // cout << typeid(img2).name() << endl;


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
    double endT = (double)getTickCount();
    cout << endT/getTickFrequency()<< endl;
    cout << "Time taken: " << (endT - startT)/getTickFrequency() << endl;
    waitKey(0); 
}

// flags - ?
UMat convert_mat_UMat(Mat img) {
    return img.getUMat(ACCESS_FAST);
}

UMat convert_mat_UMat_copy(Mat img) {
    UMat umat;
    img.copyTo(umat);
    return umat;
}


