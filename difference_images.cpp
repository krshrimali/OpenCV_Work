#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;

void invert_color(Mat src, Mat dst);

int main( int argc, char** argv ){
    Mat src; Mat dst;
    char window_name[] = "Original Image";
    char window_name_1[] = "New Image";
    char window_name_2[] = "Difference Image";

    namedWindow(window_name, CV_WINDOW_AUTOSIZE);
    namedWindow(window_name_1, CV_WINDOW_AUTOSIZE);
    namedWindow(window_name_2, CV_WINDOW_AUTOSIZE);

    src = imread( argv[1], 1 );
    dst = imread( argv[2], 1 );

    cout << src.size() << endl;
    resize(dst, dst, src.size(), INTER_LINEAR);

    cout << dst.size() << endl;

    while (true)
    {
        imshow(window_name, src);
        imshow(window_name_1, dst);

        Mat diff = src - dst;
        invert_color(diff, diff);
        imshow(window_name_2, diff);

        int c = waitKey( 20 );
        if( c == 27 ) { break; }
    }
 }

  void invert_color(Mat src, Mat dst) {
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
    // return dst;
}