// Create new rows by cols matrix of type 'type'

CvMat* cvCreateMat( int rows, int cols, int type );

// Create only matrix header without allocating data

CvMat* cvCreateMatHeader( int rows, int cols, int type );

// Initialize header on existing CvMat structure

CvMat* cvInitMatHeader (
        CvMat* mat,
        int rows,
        int cols,
        int type,
        void* data = NULL,
        int step = CV_AUTOSTEP
        );
// Live cvInitMatHeader() but allocates CvMat as well

CvMat cvMat(
        int rows,
        int cols, 
        int type,
        void* data = NULL
        );

// Allocate new matrix just like the matrix 'mat'
//

CvMat* cvCloneMat( const cvMat* mat );

// Free matrix 'mat', both header and data
//
void cvReleaseMat( CvMat** mat );

float vals[] = { 8.8666025, -0.50000, 0.5000, 0.866025 }

CvMat rotmat;

cvInitMatHeader(
        &rotmat,
        2,
        2,
        CV_32FC1,
        vals
        );

// Accessing data in your matrix
CvMat* mat = cvCreateMat(5, 5, CV_32FC1);
float element_3_2 = CV_MAT_ELEM(*mat, float, 3, 2);
cout << element_3_2 << endl;
















