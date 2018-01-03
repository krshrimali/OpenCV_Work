#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

int main( int argc, char** argv ){
  int MAX_KERNEL_LENGTH = 31;

  Mat src; Mat dst;
  char window_name[] = "Original Image";
  char window_name_1[] = "Blured Image";
  char window_name_2[] = "Detailed Image";

  namedWindow( window_name, CV_WINDOW_AUTOSIZE );
  namedWindow(window_name_1, CV_WINDOW_AUTOSIZE);
  namedWindow(window_name_2, CV_WINDOW_AUTOSIZE);

  /// Load the source image
  src = imread( argv[1], 1 );
  imshow(window_name, src);

  dst = src.clone();
  for ( int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2 ){
    blur( src, dst, Size( i, i ), Point(-1, -1));
  }  
  imshow(window_name_1, dst);
  
  Mat diff = src - dst;
  imshow(window_name_2, diff );

  int c = waitKey( 20 );
  if( c == 27 ) { return -1; }
  return 0;
 }