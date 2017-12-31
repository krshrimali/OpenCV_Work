#include <opencv2/opencv.hpp>
#include "highgui.h"
// #include "imgproc.h"
// #include  "/usr/include/opencv2/imgproc/imgproc.hpp"

// #include  <iostream>
// #include  <iomanip>
// #include <cstdlib>

// using namespace std;
// using namespace cv;

// Function declaration
void changeQuantisationGray(Mat, int);
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
        // Mat gray = Mat(size, CV_8UC1);
        // cvtColor(cameraFrame, gray, CV_BGR2GRAY);
    //         Mat smallImg = Mat(smallSize, CV_8UC3);
    // resize(srcColor, smallImg, smallSize, 0, 0, INTER_LINEAR);

        Mat gray;
        resize(cameraFrame, gray, Size(cameraFrame.cols, cameraFrame.rows));
        changeQuantisationGray(gray, 8);
        imshow("New", gray);

        char keyPress = waitKey(20);
        if(keyPress == 27) break;
    }
}

void changeQuantisationGray(Mat image, int num_bits) {
    image = Mat(image.size(), CV_8UC1);
    assert( (image.type() == CV_8UC1) && (num_bits >= 1) && (num_bits <= 8) );
    cout << "Check" << endl;
    uchar mask = 0xFF << (8 - num_bits);
    for(int row = 0; row < image.rows; row++) {
        for(int col = 0; col < image.cols; col++) {
            image.at<uchar>(row, col) = image.at<uchar>(row, col) & mask;
        }   
    }
}