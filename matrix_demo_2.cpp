#include "cv.h"
#include "highgui.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
    cout << "Size of matrix: " << endl;
    int size;
    cin >> size;
    CvMat* mat = cvCreateMat( size, size, CV_32FC3 );
    float element_3_2 = 7.7;
    *( (float*)CV_MAT_ELEM_PTR( *mat, 3, 2 ) ) = element_3_2;
    int N = mat->height;

    cout << CV_MAT_ELEM(*mat, float, 3, 2) << endl;
    // Output matrix: 
    cout << "\t    Matrix Outut" << endl;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cout << CV_MAT_ELEM(*mat, float, i, j) << "\t";
        }
        cout << endl;
    }

    // take input from user
    float x;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++) {
            cin >> x;
            *( (float*)CV_MAT_ELEM_PTR(*mat, i, j) ) = x;
        }
    }
    
    // Output Matrix(new): 
    cout << "\t    OUTPUT MATRIX(NEW)" << endl;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            cout << CV_MAT_ELEM(*mat, float, i, j) << "\t";
        }
        cout << endl;
    }
    return 0;
}

