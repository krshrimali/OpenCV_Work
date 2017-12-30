#include "cv.h"
#include "highgui.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void about_image(IplImage* image) {
    cout << image->width << image->height << endl;
}
void example2_4(IplImage* image, int channel){
    // create some windows to show input
    // and output images in.

    cvNamedWindow("Example4-in");
    cvNamedWindow("Example4-out");
    cvShowImage("Example4-in", image);
    // cout << cvGetSize(image);

    IplImage* out = cvCreateImage(
            cvGetSize(image),
            IPL_DEPTH_8U,
            (int) channel
            );
    cvSmooth(image, out, CV_GAUSSIAN, 3, 3);

    cvShowImage("Example4-out", out);

    cvReleaseImage(&out);

    cvWaitKey(0);
    cvDestroyWindow("Example4-in");
    cvDestroyWindow("Example4-out");
}

int main(int argc, char** argv){
    IplImage* image = cvLoadImage(argv[1]);
    int channel = atoi(argv[2]);
    example2_4(image, channel);
    cout << "That's it." << endl;
}
