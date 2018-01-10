#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    if(argc != 2) {
        cerr << "Incorrect arguments" << endl;
        exit(1);
    }
    Mat src = imread(argv[1], 1);
    Mat dst = imread(argv[2], 1);

    cout << src.at<char>(300, 400) << endl;
    cout << dst.at<char>(300, 400) << endl;

}