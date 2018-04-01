#include <opencv2/opencv.hpp>
#include "highgui.h"
// #include "imgproc.h"
// #include  "/usr/include/opencv2/imgproc/imgproc.hpp"

#include  <iostream>
// #include  <iomanip>

using namespace std;
using namespace cv;

// Function declaration
void cartoonifyImage(Mat, Mat, int, int);
// void evilify(Mat src, Mat dst);
void removePepperNoise(Mat &mask);
void draw_face(Mat dst);

int main(int argc, char** argv) {
    // int cameraNo = 0; // default
    // if(argc > 1)
    //     cameraNo = atoi(argv[1]); // convert character to int for cameraNo
    // // get access to camera
    
    // VideoCapture camera;
    // camera.open(cameraNo);

    // if(!camera.isOpened()) {
    //     cerr << "Error: Could not access camera/video" << endl;
    //     exit(1);
    // }
    // camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    // camera.set(CV_CAP_PROP_FRAME_WIDTH, 480);

    // while(true) {
    //     // grab next camera 
    Mat cameraFrame = imread(argv[1], 1);
    // Mat cameraFrame;
    // camera >> cameraFrame;
    if(cameraFrame.empty()) {
        cerr << "ERROR: Couldn't grab a camera frame." << endl;
        exit(1);
    }
    // create blank image that we will draw into
    imshow("original", cameraFrame);
    Mat displayedFrame(cameraFrame.size(), CV_8UC3); // 8 bit unsigned, 3 channels
    // Mat newdisplayFrame(cameraFrame.size(), CV_8UC3);
    cartoonifyImage(cameraFrame, displayedFrame, 0, 0);
    // draw_face(displayedFrame);
    // evilify(cameraFrame, newdisplayFrame);

    // imshow("Evilifier", newdisplayFrame);
    imshow("Cartoonifier", displayedFrame);

    char keyPress = cvWaitKey(1); // wait 20 ms
    if(keyPress == 27) { return 0; } // esc
    // } // end while
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
    // cout << "Scalar color is: " << color;

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

    // Bottom lip of mouth
    int mouthY = faceH * 48/100;
    int mouthW = faceW * 45/100;
    int mouthH = faceH * 6/100;
    ellipse(faceOutline, Point(sw/2, sh/2 + mouthY), Size(mouthW, mouthH), 0, 0, 180, color,
    thickness, CV_AA);
    
    // put text
    int fontFace = FONT_HERSHEY_COMPLEX;
    float fontScale = 1.0f;
    int fontThickness = 2;
    char* szMsg = "Put your face here";
    putText(faceOutline, szMsg, Point(sw * 23/100, sh * 10/100), fontFace, fontScale, color,
    fontThickness, CV_AA);
    // overlay with alpha value
    addWeighted(dst, 1.0, faceOutline, 0.7, 0, dst, CV_8UC3);
}
void cartoonifyImage(Mat srcColor, Mat dst, int evilify, int alienMode) {
    // Median Blurring and grayscaling

    Mat gray; // make gray image - blank
    cvtColor(srcColor, gray, CV_BGR2GRAY);
    // imshow("grayscaled", gray);
    const int MEDIAN_BLUR_FILTER_SIZE = 7;
    medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);
    
    // imshow("blurred_image", gray);
    
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
        // imshow("edges", edges);
        medianBlur(masking, masking, 3);
        // imshow("masking", masking);
        // dst = masking;
        // dst.copyTo(dst, masking);  
    } else {
        const int LAPLACIAN_FILTER_SIZE = 5;
        Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
        // imshow("edges_2", edges);
        // Mat masking;
        // imshow("laplacian", edges);
        const int EDGES_THRESHOLD = 80;
        threshold(edges, masking, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);
        // imshow("thresholded", masking);
        removePepperNoise(masking);
        // imshow("edges_3", masking);
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
        int ksize=9;
        double sigmaColor = 9;
        double sigmaSpace = 7;
        bilateralFilter(smallImg, tmp, ksize, sigmaColor, sigmaSpace);
        bilateralFilter(tmp, smallImg, ksize, sigmaColor, sigmaSpace);
        imshow("Filtered", smallImg);
    }

    if(alienMode) {
        Mat yuv = Mat(smallSize, CV_8UC3);
        cvtColor(smallImg, yuv, CV_BGR2YCrCb);

        int sw = smallSize.width;
        int sh = smallSize.height;
        Mat mask, maskPlusBorder;
        maskPlusBorder = Mat::zeros(sh + 2, sw + 2, CV_8UC1);
        mask = maskPlusBorder(Rect(1, 1, sw, sh));

        resize(edges, mask, smallSize);
        // imshow("edges4", edges);
        // imshow("mask5", mask);
        const int EDGES_THRESHOLD = 80;
        threshold(mask, mask, EDGES_THRESHOLD, 255, THRESH_BINARY);
        dilate(mask, mask, Mat());
        erode(mask, mask, Mat());

        int const NUM_SKIN_POINTS = 6;
        Point skinPts[NUM_SKIN_POINTS];
        skinPts[0] = Point(sw/2, sh/2 - sh/6);
        skinPts[1] = Point(sw/2 - sw/11, sh/2 - sh/6);
        skinPts[2] = Point(sw/2 + sw/11, sh/2 - sh/6);
        skinPts[3] = Point(sw/2, sh/2 + sh/16);
        skinPts[4] = Point(sw/2 - sw/9, sh/2 + sh/16);
        skinPts[5] = Point(sw/2 + sw/9, sh/2 + sh/16);

        const int LOWER_Y = 60;
        const int UPPER_Y = 80;
        const int LOWER_Cr = 25;
        const int UPPER_Cr = 15;
        const int LOWER_Cb = 20;
        const int UPPER_Cb = 15;
        Scalar lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
        Scalar upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);

        const int CONNECTED_COMPONENTS = 4; // To fill diagonally, use 8.
        const int flags = CONNECTED_COMPONENTS | FLOODFILL_FIXED_RANGE \
        | FLOODFILL_MASK_ONLY;
        Mat edgeMask = mask.clone(); // Keep a copy of the edge mask.
        // "maskPlusBorder" is initialized with edges to block floodFill().
        for (int i=0; i< NUM_SKIN_POINTS; i++) {
            floodFill(yuv, maskPlusBorder, skinPts[i], Scalar(), NULL,
            lowerDiff, upperDiff, flags);
        }
        mask -= edgeMask;

        int Red = 0;
        int Green = 0;
        int Blue = 0;
        add(smallImg, Scalar(Blue, Green, Red), smallImg, mask);
        // imshow("smallImg", smallImg);
        // imshow("yuv", edgeMask);
    }
    // Mat bigImg = Mat(size, CV_8U);
    resize(smallImg, srcColor, size, 0, 0, INTER_LINEAR);
    memset((char*)dst.data, 0, dst.step * dst.rows);
    // dst.setTo(0);
    srcColor.copyTo(dst, masking);
    // return dst;
    // return mask_;
}

/* Code referred from Master OpenCV Book */
void removePepperNoise(Mat &mask)
{
    // For simplicity, ignore the top & bottom row border.
    for (int y=2; y<mask.rows-2; y++) {
        // Get access to each of the 5 rows near this pixel.
        uchar *pThis = mask.ptr(y);
        uchar *pUp1 = mask.ptr(y-1);
        uchar *pUp2 = mask.ptr(y-2);
        uchar *pDown1 = mask.ptr(y+1);
        uchar *pDown2 = mask.ptr(y+2);

        // For simplicity, ignore the left & right row border.
        pThis += 2;
        pUp1 += 2;
        pUp2 += 2;
        pDown1 += 2;
        pDown2 += 2;
        for (int x=2; x<mask.cols-2; x++) {
            uchar v = *pThis;   // Get the current pixel value (either 0 or 255).
            // If the current pixel is black, but all the pixels on the 2-pixel-radius-border are white
            // (ie: it is a small island of black pixels, surrounded by white), then delete that island.
            if (v == 0) {
                bool allAbove = *(pUp2 - 2) && *(pUp2 - 1) && *(pUp2) && *(pUp2 + 1) && *(pUp2 + 2);
                bool allLeft = *(pUp1 - 2) && *(pThis - 2) && *(pDown1 - 2);
                bool allBelow = *(pDown2 - 2) && *(pDown2 - 1) && *(pDown2) && *(pDown2 + 1) && *(pDown2 + 2);
                bool allRight = *(pUp1 + 2) && *(pThis + 2) && *(pDown1 + 2);
                bool surroundings = allAbove && allLeft && allBelow && allRight;
                if (surroundings == true) {
                    // Fill the whole 5x5 block as white. Since we know the 5x5 borders
                    // are already white, just need to fill the 3x3 inner region.
                    *(pUp1 - 1) = 255;
                    *(pUp1 + 0) = 255;
                    *(pUp1 + 1) = 255;
                    *(pThis - 1) = 255;
                    *(pThis + 0) = 255;
                    *(pThis + 1) = 255;
                    *(pDown1 - 1) = 255;
                    *(pDown1 + 0) = 255;
                    *(pDown1 + 1) = 255;
                }
                // Since we just covered the whole 5x5 block with white, we know the next 2 pixels
                // won't be black, so skip the next 2 pixels on the right.
                pThis += 2;
                pUp1 += 2;
                pUp2 += 2;
                pDown1 += 2;
                pDown2 += 2;
            }
            // Move to the next pixel.
            pThis++;
            pUp1++;
            pUp2++;
            pDown1++;
            pDown2++;
        }
    }
}