#include "cv.h"
#include "highgui.h"
#include <cstdlib>

using namespace std;
int main(int argc, char** argv) {
    if(argc == 1) {
        cerr << "Image not given. " << endl;
        exit(1);
    }
    else {
        IplImage* img = cvLoadImage( argv[1] );
        cvRectange(img, cvPoint(5,10), cvPoint(20,30), cvScalar(255, 255, 255)
                );
    }
    cvNamedWindow("Image");
    cvShowImage("Image", img);
    cvReleaseImage(&img);

    cvWaitKey(0);
    cvDestroyWindow("Image");
    return 0;
}
