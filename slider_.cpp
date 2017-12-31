#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace std;

int g_slider_position = 0;
CvCapture* g_capture = NULL;

void onTrackbarSlide(int pos) {
    cvSetCaptureProperty(
            g_capture,
            CV_CAP_PROP_POS_FRAMES,
            pos);
}

int main( int argc, char** argv) {
    cvNamedWindow("Example3", CV_WINDOW_AUTOSIZE);
    //g_capture = cvCreateFileCapture(argv[1]);
    IplImage* g_capture = cvLoadImage(argv[1]);
    cout << g_capture << endl;
    if(g_capture) {
        cout << "Doing" << endl;
    }
    int frames = (int) cvGetCaptureProperty(
            g_capture,
            CV_CAP_PROP_FRAME_COUNT);
    if(frames != 0) {
        cvCreateTrackbar(
                "Position", 
                "Example3", 
                &g_slider_position, 
                frames,
                onTrackbarSlide
                );
    }
    IplImage* frame;
    //frame = cvLoadImage(argv[1]);
    cvShowImage("Example1", frame);
    cvWaitKey(0);
    cvReleaseImage(&frame);
    cvDestroyWindow("Example3");
    return 0;
}
