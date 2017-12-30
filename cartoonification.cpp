#include <opencv2/opencv.hpp>
#include "highgui.h"
// #include "imgproc.h"
// #include  "/usr/include/opencv2/imgproc/imgproc.hpp"


#include  <vector>
#include  <iostream>
// #include  <iomanip>

using namespace std;
using namespace cv;

// Function declaration
void cartoonifyImage(Mat srcColor, Mat dst, int);
// void evilify(Mat src, Mat dst);
void draw_face(Mat dst);

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
        Mat displayedFrame(cameraFrame.size(), CV_8UC3); // 8 bit unsigned, 3 channels
        // Mat newdisplayFrame(cameraFrame.size(), CV_8UC3);
        cartoonifyImage(cameraFrame, displayedFrame, 0);
        draw_face(displayedFrame);
        // evilify(cameraFrame, newdisplayFrame);

        // imshow("Evilifier", newdisplayFrame);
        imshow("Cartoonifier", displayedFrame);

        char keyPress = cvWaitKey(1); // wait 20 ms
        if(keyPress == 27) { break; } // esc
    } // end while
}
// Showing user where to put face
void draw_face(Mat dst) {
    // ellipse covering 70 percent of img height
    // fixed aspect ratio of 0.72 - face will not become too skinny or fat

    // Draw color face on a black background
    Size size = dst.size();
    Mat faceOutline = Mat::zeros(size, CV_8UC3);
    Scalar color = CV_RGB(255, 255, 0); // YELLOW 
    int thickness = 4;

    int sh = size.height;
    int sw = size.width;

    int faceH = sh/2 * 70/100; // faceH - radius of ellipse, half of 0.7*sh
    int faceW = faceH * 72/100;

    // Draw face outline
    ellipse(faceOutline, Point(sw/2, sh/2), Size(faceW, faceH), 0, 0, 360, color, thickness, CV_AA);
    cout << "Scalar color is: " << color;

    // 2 eye arcs 
    int eyeW = faceW * 23/100;
    int eyeH = faceH * 11/100;
    int eyeX = faceW * 48/100;
    int eyeY = faceH * 13/100;

    Size eyeSize = Size(eyeW, eyeH);
    // Set angle and shift for eye half ellipses
    int eyeA = 15; // angle in degrees
    int eyeYshift = 11;

    // draw top of right eye
    ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY), eyeSize, 0, 180+eyeA, 360-eyeA, color 
    , thickness, CV_AA);
    // draw bottom of right eye
    ellipse(faceOutline, Point(sw/2 - eyeX, sh/2 - eyeY - eyeYshift), eyeSize, 0, 0+eyeA, 180-eyeA, color 
    , thickness, CV_AA);
    // draw top of left eye
    ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY), eyeSize, 0, 180+eyeA, 360-eyeA, color 
    , thickness, CV_AA);
    // draw bottom of left eye
    ellipse(faceOutline, Point(sw/2 + eyeX, sh/2 - eyeY - eyeYshift), eyeSize, 0, 0+eyeA, 180-eyeA, color 
    , thickness, CV_AA);
    
    // overlay with alpha value
    addWeighted(dst, 1.0, faceOutline, 0.7, 0, dst, CV_8UC3);
}
void cartoonifyImage(Mat srcColor, Mat dst, int evilify) {
    Mat gray; // make gray image - blank
    cvtColor(srcColor, gray, CV_BGR2GRAY);

    const int MEDIAN_BLUR_FILTER_SIZE = 7;
    medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);

    Size size = srcColor.size();
    Mat masking = Mat(size, CV_8U);
    Mat edges = Mat(size, CV_8U);

    if(evilify) {
        Mat edges, edges2;
        Scharr(gray, edges, CV_8U, 1, 0);
        Scharr(gray, edges2, CV_8U, 1, 0, -1);

        edges += edges2;
        const int EVIL_EDGE_THRESHOLD = 12;
        threshold(edges, masking, EVIL_EDGE_THRESHOLD, 255, THRESH_BINARY_INV);
        imshow("edges", edges);
        medianBlur(masking, masking, 3);
        imshow("masking", masking);
        // dst = masking;
        // dst.copyTo(dst, masking);  
    } else {
        const int LAPLACIAN_FILTER_SIZE = 5;
        Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
        imshow("edges_2", edges);
        // Mat masking;
        const int EDGES_THRESHOLD = 80;
        threshold(edges, masking, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);
        imshow("edges_3", masking);
    }
    // reduce pixels size by quarter - to make acceptable speed for bilateral filter
    Size smallSize;
    smallSize.width = size.width/2;
    smallSize.height = size.height/2;

    Mat smallImg = Mat(smallSize, CV_8UC3);
    resize(srcColor, smallImg, smallSize, 0, 0, INTER_LINEAR);

    Mat tmp = Mat(smallSize, CV_8UC3);
    int repetitions = 7;

    for(int i = 0; i<repetitions; i++) {
        int ksize=21;
        double sigmaColor = 9;
        double sigmaSpace = 7;
        bilateralFilter(smallImg, tmp, ksize, sigmaColor, sigmaSpace);
        bilateralFilter(tmp, smallImg, ksize, sigmaColor, sigmaSpace);
    }

    // Mat bigImg = Mat(size, CV_8U);
    resize(smallImg, srcColor, size, 0, 0, INTER_LINEAR);
    memset((char*)dst.data, 0, dst.step * dst.rows);
    // dst.setTo(0);
    srcColor.copyTo(dst, masking);
    // return dst;
    // return mask_;
}