// Program to convert image to pixel matrix
// Experiments on blurred images (mean blur) - to understand it's working and effects on pixel intensity
// Credits: Kushashwa Ravi Shrimali
// Dr. Shyama Prasad Mukherjee International Institute of Information Technology, Naya Raipur
// Contact: kushashwa16100@iiitnr.edu.in
#include "cv.h"
#include <opencv2/opencv.hpp>
#include "highgui.h"
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

CvMat* create_matrix(char*, Mat&);

CvMat* create_matrix(char* name, Mat& dst) {
    CvMat* mat = cvCreateMat( dst.rows, dst.cols, CV_32FC1 );
    cout << dst.rows << endl;
    cout << dst.cols << endl;

    cout << "Pixel Matrix for given image: " << name << endl;
    for(int row = 0; row < dst.rows; row++) {
        for(int col = 0; col < dst.cols; col++) {
            Vec3b intensity = dst.at<Vec3b>(row, col);
            // cout << "Row is: " << row << " Col is: " << col << endl;
            // for(int i = 0; i < 1; i++) {
            int element = (int)intensity.val[0];
            *((int*)CV_MAT_ELEM_PTR(*mat, row, col) ) = element;
            // cout << (int)intensity.val[i] << endl;
            // }
        }
    }
    
    for(int i = 0; i < dst.rows; i++) {
        for(int j = 0; j < dst.cols; j++) {
            cout << CV_MAT_ELEM(*mat, int, i, j) << "\t";
        }
        cout << endl;
    }
    cout << "Printed" << endl;
    return mat;
}
int main(int argc, char** argv) {
    if(argc < 2) {
        cerr << "Incorrect arguments. Input image address." << endl;
        return -1;
    }
    Mat src = imread(argv[1]);
    // cout << src.size() << endl;
    Size size = src.size();
    // cout << size[0] << size[1] << endl;
    cout << size<< endl;

    Size new_size;
    new_size.width = src.size().width/40;
    new_size.height = src.size().height/40;
    
    Mat new_image = Mat(new_size, src.type());
    resize(src, new_image, new_size, 0, 0, INTER_LINEAR);

    cout << src.size() << endl;
    cout << new_image.size() << endl;

    Mat dst = new_image.clone();
    for ( int i = 1; i < 31; i = i + 2 ){
        blur( new_image, dst, Size( i, i ), Point(-1, -1));
    }
    
    // CvMat* mat1 = create_matrix("new image", new_image);

    CvMat* mat = create_matrix("blurred image", dst);

    /* creating a blank image (black)
    writing matrix onto it 
    in progress
    */
    Mat blank = Mat(new_image.size().width*2, new_image.size().height*2, new_image.type(), Scalar(0, 0, 0));
    
    for(int i = 0; i < new_image.rows/2; i++){
        for(int j = 0; j < new_image.cols/2; j++){
            int k = i + 10;
            int l = j + 10;
            int element = CV_MAT_ELEM(*mat, int, i, j);

            string s = to_string(element);
            putText(blank, s, cvPoint(k,l),
            FONT_HERSHEY_COMPLEX_SMALL, 1.0, cvScalar(255, 255, 255), 1, CV_AA);
        }
    }

    cout << "Value is: " << CV_MAT_ELEM(*mat, int, 10, 10) << endl;

    imwrite("/home/kushashwaravishrimali/Documents/projects/December2k17/opencv_cpp/opencv_work/blank_image.png", blank);

    while(true) {
        imshow("new_image", new_image);
        imshow("dst", dst);
        imshow("blank", blank);

        int c = waitKey(20);
        if(c == 27) {
            break;
        }
    }
    // return 0;
}