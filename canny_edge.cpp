// output image - canny edge

#include "highgui.h"
#include "cv.h"
#include <iostream>
#include <cstdlib>

using namespace std;

IplImage* doCanny(
        IplImage* in, 
        double lowThresh,
        double highThresh,
        double aperture
        ) {
    // if(in->nChannels != 1){
    //     cerr << "Channels should be 1 of the image " << endl;
    //     return (0./); // Canny needs only gray scale images
    // }

    IplImage* out = cvCreateImage(
            cvSize(in->width, in->height),
            IPL_DEPTH_8U,
            1 
            );
    cvCanny(in, out, lowThresh, highThresh, aperture);
    return (out);
    };

int main(int argc, char** argv) {
    if(argc == 1) 
        exit(0);  
    IplImage* image = cvLoadImage(argv[1]);
    // cout << image->width << image->height << endl;
    int low_thresh = atoi(argv[2]);
    int high_thresh = atoi(argv[3]);

    cout << low_thresh << high_thresh << endl;

    IplImage* output_image = doCanny(image, low_thresh, high_thresh, 3);
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
