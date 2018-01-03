#include <iostream>

void cv::blur( InputArray src, OutputArray dst, Size ksize, Point anchor, int borderType) {
    CV_INSTRUMENT_REGION();
    boxFilter(src, dst, -1, ksize, anchor, true, borderType);
}