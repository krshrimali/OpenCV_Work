#include <opencv2/opencv.hpp>
#include "highgui.h"
// #include "imgproc.h"
// #include  "/usr/include/opencv2/imgproc/imgproc.hpp"

#include  <iostream>
#include  <iomanip>
#include <cstdlib>

using namespace std;
using namespace cv;

// Function declaration
void changeQuantisationGray(Mat&, int);

// For sampling
void sampling_higher_resolution(Mat&, float);

// invert color
void invert_color(Mat& src, Mat& dst);

int main(int argc, char** argv) {
    int cameraNo = 0;
    if(argc > 1)
        cameraNo = atoi(argv[1]); // convert character to int for cameraNo
    // get access to camera
    
    VideoCapture camera;
    camera.open(cameraNo);

    if(!camera.isOpened()) {
        cerr << "Error: Could not access camera/video" << endl;
        exit(1);
    }
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 480);

    while(true) {
        // grab next camera 
        Mat cameraFrame;
        camera >> cameraFrame;
        if(cameraFrame.empty()) {
            cerr << "ERROR: Couldn't grab a camera frame." << endl;
            exit(1);
        }
        // create blank image that we will draw into
        imshow("original", cameraFrame);
        Size size = cameraFrame.size();
        

        Mat gray;
        resize(cameraFrame, gray, Size(cameraFrame.cols, cameraFrame.rows));
        // changeQuantisationGray(gray, 8);
        Mat displayFrame = Mat(size, CV_8UC3);
        invert_color(cameraFrame, displayFrame);

        // float resolution_count = 2.0; // or cin >> resolution_count;
        if(argc > 1) {
            float resolution_count = atof(argv[1]);
            cout << resolution_count << endl;
            sampling_higher_resolution(cameraFrame, resolution_count);
        }
        imshow("New", gray);

        char keyPress = waitKey(20);
        if(keyPress == 27) break;
    }
}

void changeQuantisationGray(Mat &image, int num_bits) {
    image = Mat(image.size(), CV_8UC1);
    assert( (image.type() == CV_8UC1) && (num_bits >= 1) && (num_bits <= 8) );
    uchar mask = 0xFF << (8 - num_bits);

    for(int row = 0; row < image.rows; row++) {
        for(int col = 0; col < image.cols; col++) {
            image.at<uchar>(row, col) = image.at<uchar>(row, col) & mask;
        }   
    }
    imshow("img", image);
}

void sampling_higher_resolution(Mat &image, float resolution_count) {
    Size size = image.size();
    Mat higher_image = Mat(size, CV_8UC3);
    resize(image, higher_image, Size(image.cols * resolution_count, image.rows * resolution_count));
    imshow("higher image", higher_image);
}

void invert_color(Mat& src, Mat& dst) {
    CV_Assert ( src.type() == CV_8UC3 );
    dst = src.clone();
    for(int row=0; row<src.rows; row++) {
        for(int col=0; col<src.cols; col++) {
            for(int channel=0; channel<src.channels(); channel++) {
                dst.at<Vec3b>(row,col)[channel] = 255 - src.at<Vec3b>(row,col)[channel];
            }
        }
    }
    imshow("dst", dst);
}