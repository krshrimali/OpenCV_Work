// output image - half width, half height
#include "highgui.h"
#include "cv.h"
#include <iostream>

using namespace std;

IplImage* doPyrDown(
        IplImage* in
        ) {
    // best to make sure input is divisible by two
    //
    assert(in->width%2 == 0 && in->height%2 == 0);
    IplImage* out = cvCreateImage(
            cvSize(in->width/2, in->height/2),
            in->depth,
            in->nChannels
            );
    cvPyrDown(in, out);
    return out;
};

int main(int argc, char** argv){
    IplImage* image = cvLoadImage(argv[1]);
    IplImage* output_image = doPyrDown(image);
    cvNamedWindow("Input Image");
    cvNamedWindow("Output Image");
    cvShowImage("Input Image", image);
    cvShowImage("Output Image", output_image);

    cvReleaseImage(&image);
    cvReleaseImage(&output_image);
    cvWaitKey(0);
    cvDestroyWindow("Input Image");
    cvDestroyWindow("Output Image");
    return 0;
}
