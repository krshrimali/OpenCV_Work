#include <iostream>


typedef struct CvMat {
    int type;
    int step;
    int* refcount; // internal use
    union {
        uchar* ptr;
        short* s;
        int* i;
        float* fl;
        double* db;
    } data;
    union {
        int rows;
        int height;
    };
    union {
        int cols;
        int width;
    };
}CvMat;
