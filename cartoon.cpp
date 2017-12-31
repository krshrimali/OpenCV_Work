#include "cv.h"
#include <iostream>

using namespace cv;
using namespace std;

Mat cartoonifyImage() {
    Mat gray; // make gray image - blank
    cvtColor(srcColor, gray, CV_BGR2GRAY);
    const int MEDIAN_BLUR_FILTER_SIZE = 7;
    medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);

    Mat edges;
    const int LAPLACIAN_FILTER_SIZE = 5;
    Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);

    Mat mask;
    const int EDGES_THRESHOLD = 80;
    threshold(edges, mask, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);

    return edges;
}