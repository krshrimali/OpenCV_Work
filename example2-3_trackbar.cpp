#include "cv.h"
#include "highgui.h"

int g_slider_position = 0;
CvCapture* g_capture = NULL;

void onTrackbarSlide(int pos) {
    cvSetCaptureProperty(
            g_capture,
            CV_CAP_PROP_POS_FRAMES,
            pos
            );
}

int main( int argc, char** argv) {
    if(argc != 2){
        return -1;
    }
    cvNamedWindow("Example3", CV_WINDOW_AUTOSIZE);
    g_capture = cvCreateFileCapture(argv[1]);
    int frames = (int) cvGetCaptureProperty(
            g_capture,
            CV_CAP_PROP_FRAME_COUNT
            );
    if(frames != 0){
        cvCreateTrackbar(
                "Position", 
                "Example3", 
                &g_slider_position,
                frames,
                onTrackbarSlide
                );
    }
    IplImage* frame;

