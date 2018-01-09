// Source: https://stackoverflow.com/questions/31370459/how-can-i-get-the-position-and-draw-rectangle-using-opencv/31376558#31376558
// for drawing rectangle

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;
using namespace cv;

vector<Rect> rects;
bool bDraw;
Rect r; // declare rectangle vector r
Point base;

Mat3b img;
Mat3b layer;
Mat3b working;

void hist_image(Mat);
void cartoonifyImage(Mat, Mat, int, int);
// void removePepperNoise(Mat);
void CallBackFunc(int, int, int, int, void*);

void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if ( event == EVENT_LBUTTONDOWN )
    {
        // cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

        // Init your rect
        base.x = x;
        base.y = y;
        r.x = x;
        r.y = y;
        r.width = 0;
        r.height = 0;
        // cout << "Vector: " << r << endl;
        bDraw = true;
    }
    else if ( event == EVENT_MOUSEMOVE )
    {
        // cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

        // If drawing, update rect width and height
        if(!bDraw) return;

        int dx = abs(r.x - x);
        int dy = abs(r.y - y);

        if(x < base.x) {
            r.x = x;
            r.width = abs(x - base.x);
        } else {
            r.width = dx;
        }

        if(y < base.y) {
            r.y = y;
            r.height = abs(y - base.y);
        } else {
            r.height = dy;
        }

        // Refresh
        working = layer.clone();
        cout << r.x << endl;
        cout << r.y << endl;
        // cout << "Vector: " << r << endl;
        rectangle(working, r, Scalar(0,255,0));
        imshow("My Window", working);
    }
    else if ( event == EVENT_LBUTTONUP)
    {
        cout << "Left button released" << endl;

        // Save rect, draw it on layer
        rects.push_back(r);
        cout << r << endl;
        rectangle(layer, r, Scalar(0,255,255));
        Rect rec(r.x, r.y, r.width, r.height);
        r = Rect();
        bDraw = false;
        // Refresh
        working = layer.clone();
        // cout << "Vector: " << r << endl;
        rectangle(working, r, Scalar(0,255,0));
        // imshow("My Window", working);

        Mat gray_src;
        cvtColor(working, gray_src, COLOR_RGB2GRAY);
        // play with roi image now

        Mat blur_image;
        for ( int i = 1; i < 31; i = i + 2 )
        {
          blur( gray_src, blur_image, Size( i, i ), Point(-1,-1) );
        }

        Mat dst = blur_image(rec);
        Mat dst_non_gray = working(rec);

        hist_image(dst_non_gray);

        // int cartoonIF, threshIF;
        // cout << "Want cartoon image? " << endl;
        // cin >> cartoonIF;
        // cout << "Want thresholded image? " << endl;
        // cin >> threshIF;
        int cartoonIF = 0;
        int threshIF = 1;


        if(cartoonIF){
          Mat cartoonififed_image = Mat(dst_non_gray.size(), CV_32FC3);
          cartoonifyImage(dst_non_gray, cartoonififed_image, 0, 0);
        }

        if(threshIF) {
          CvMat* mat = cvCreateMat( dst.rows, dst.cols, CV_32FC1 );
          cout << dst.rows << endl;
          cout << dst.cols << endl;

          int sum = 0;
          cout << "dst.channels " << dst.channels() << endl;
          for(int row = 0; row < dst.rows; row++) {
            for(int col = 0; col < dst.cols; col++) {
              Vec3b intensity = dst.at<Vec3b>(row, col);
              int element = (int)intensity.val[0];
              sum += element;
              // cout << element << endl;
            }
          }

          float avg = (float)sum / (dst.rows * dst.cols);
          cout << avg << endl;

          Mat result;
          threshold(gray_src, result, avg, 255, THRESH_BINARY_INV);
          imshow("Window_dst", dst);
          imshow("Window_result", result);
        }
    }
}

string type2str(int type) {
  string r;

  uchar depth = type & CV_MAT_DEPTH_MASK;
  uchar chans = 1 + (type >> CV_CN_SHIFT);

  switch ( depth ) {
    case CV_8U:  r = "8U"; break;
    case CV_8S:  r = "8S"; break;
    case CV_16U: r = "16U"; break;
    case CV_16S: r = "16S"; break;
    case CV_32S: r = "32S"; break;
    case CV_32F: r = "32F"; break;
    case CV_64F: r = "64F"; break;
    default:     r = "User"; break;
  }

  r += "C";
  r += (chans+'0');

  return r;
}

void hist_image(Mat src) {
  // Mat src, hsv;
  // if( argc != 2 || !(src=imread(argv[1], 1)).data )
  //     return -1;
  // imshow("NEwwest window", src);
  string typ = type2str(src.type());
  cout << "Matrix is: " << typ.c_str() << endl;
  Size size = src.size();
  cout << size << endl;
  // cout << src.type() << endl;
  Mat hsv = Mat(size, CV_32FC3);

  cvtColor(src, hsv, CV_BGR2HSV);

  imshow("HSV Image", hsv);
  // Quantize the hue to 30 levels
  // and the saturation to 32 levels
  int hbins = 30, sbins = 32;
  int histSize[] = {hbins, sbins};
  // hue varies from 0 to 179, see cvtColor
  float hranges[] = { 0, 180 };
  // saturation varies from 0 (black-gray-white) to
  // 255 (pure spectrum color)
  float sranges[] = { 0, 256 };
  const float* ranges[] = { hranges, sranges };
  MatND hist;
  // we compute the histogram from the 0-th and 1-st channels
  int channels[] = {0, 1};

  calcHist( &hsv, 1, channels, Mat(), // do not use mask
           hist, 2, histSize, ranges,
           true, // the histogram is uniform
           false );

  double maxVal=0;
  minMaxLoc(hist, 0, &maxVal, 0, 0);

  int scale = 10;
  Mat histImg = Mat::zeros(sbins*scale, hbins*10, CV_8UC3);

  for( int h = 0; h < hbins; h++ )
      for( int s = 0; s < sbins; s++ )
      {
          float binVal = hist.at<float>(h, s);
          int intensity = cvRound(binVal*255/maxVal);
          rectangle( histImg, Point(h*scale, s*scale),
                      Point( (h+1)*scale - 1, (s+1)*scale - 1),
                      Scalar::all(intensity),
                      CV_FILLED );
      }

  // namedWindow( "Source", 1 );
  imshow( "Source", src );

  // namedWindow( "H-S Histogram", 1 );
  imshow( "H-S Histogram", histImg );
  // waitKey();

}

void cartoonifyImage(Mat srcColor, Mat dst, int evilify, int alienMode) {
    // Median Blurring and grayscaling

    Mat gray; // make gray image - blank
    cvtColor(srcColor, gray, CV_BGR2GRAY);
    // imshow("grayscaled", gray);
    const int MEDIAN_BLUR_FILTER_SIZE = 7;
    medianBlur(gray, gray, MEDIAN_BLUR_FILTER_SIZE);

    // imshow("blurred_image", gray);

    Size size = srcColor.size();
    Mat masking = Mat(size, CV_8U);
    Mat edges = Mat(size, CV_8U);

    if(evilify) {
        Mat edges, edges2;
        Scharr(gray, edges, CV_8U, 1, 0);
        Scharr(gray, edges2, CV_8U, 1, 0, -1);

        edges += edges2;
        const int EVIL_EDGE_THRESHOLD = 12;
        threshold(edges, masking, EVIL_EDGE_THRESHOLD, 255, THRESH_BINARY_INV);
        // imshow("edges", edges);
        medianBlur(masking, masking, 3);
        // imshow("masking", masking);
        // dst = masking;
        // dst.copyTo(dst, masking);
    } else {
        const int LAPLACIAN_FILTER_SIZE = 5;
        Laplacian(gray, edges, CV_8U, LAPLACIAN_FILTER_SIZE);
        // imshow("edges_2", edges);
        // Mat masking;
        // imshow("laplacian", edges);
        const int EDGES_THRESHOLD = 80;
        threshold(edges, masking, EDGES_THRESHOLD, 255, THRESH_BINARY_INV);
        // imshow("thresholded", masking);
        // removePepperNoise(masking);
        // imshow("edges_3", masking);
    }
    // reduce pixels size by quarter - to make acceptable speed for bilateral filter
    Size smallSize;
    smallSize.width = size.width/2;
    smallSize.height = size.height/2;

    Mat smallImg = Mat(smallSize, CV_8UC3);
    resize(srcColor, smallImg, smallSize, 0, 0, INTER_LINEAR);

    Mat tmp = Mat(smallSize, CV_8UC3);
    int repetitions = 7;

    for(int i = 0; i<repetitions; i++) {
        int ksize=9;
        double sigmaColor = 9;
        double sigmaSpace = 7;
        bilateralFilter(smallImg, tmp, ksize, sigmaColor, sigmaSpace);
        bilateralFilter(tmp, smallImg, ksize, sigmaColor, sigmaSpace);
        imshow("Filtered", smallImg);
    }

    if(alienMode) {
        Mat yuv = Mat(smallSize, CV_8UC3);
        cvtColor(smallImg, yuv, CV_BGR2YCrCb);

        int sw = smallSize.width;
        int sh = smallSize.height;
        Mat mask, maskPlusBorder;
        maskPlusBorder = Mat::zeros(sh + 2, sw + 2, CV_8UC1);
        mask = maskPlusBorder(Rect(1, 1, sw, sh));

        resize(edges, mask, smallSize);
        // imshow("edges4", edges);
        // imshow("mask5", mask);
        const int EDGES_THRESHOLD = 80;
        threshold(mask, mask, EDGES_THRESHOLD, 255, THRESH_BINARY);
        dilate(mask, mask, Mat());
        erode(mask, mask, Mat());

        int const NUM_SKIN_POINTS = 6;
        Point skinPts[NUM_SKIN_POINTS];
        skinPts[0] = Point(sw/2, sh/2 - sh/6);
        skinPts[1] = Point(sw/2 - sw/11, sh/2 - sh/6);
        skinPts[2] = Point(sw/2 + sw/11, sh/2 - sh/6);
        skinPts[3] = Point(sw/2, sh/2 + sh/16);
        skinPts[4] = Point(sw/2 - sw/9, sh/2 + sh/16);
        skinPts[5] = Point(sw/2 + sw/9, sh/2 + sh/16);

        const int LOWER_Y = 60;
        const int UPPER_Y = 80;
        const int LOWER_Cr = 25;
        const int UPPER_Cr = 15;
        const int LOWER_Cb = 20;
        const int UPPER_Cb = 15;
        Scalar lowerDiff = Scalar(LOWER_Y, LOWER_Cr, LOWER_Cb);
        Scalar upperDiff = Scalar(UPPER_Y, UPPER_Cr, UPPER_Cb);

        const int CONNECTED_COMPONENTS = 4; // To fill diagonally, use 8.
        const int flags = CONNECTED_COMPONENTS | FLOODFILL_FIXED_RANGE \
        | FLOODFILL_MASK_ONLY;
        Mat edgeMask = mask.clone(); // Keep a copy of the edge mask.
        // "maskPlusBorder" is initialized with edges to block floodFill().
        for (int i=0; i< NUM_SKIN_POINTS; i++) {
            floodFill(yuv, maskPlusBorder, skinPts[i], Scalar(), NULL,
            lowerDiff, upperDiff, flags);
        }
        mask -= edgeMask;

        int Red = 0;
        int Green = 0;
        int Blue = 0;
        add(smallImg, Scalar(Blue, Green, Red), smallImg, mask);
        imshow("smallImg", smallImg);
        imshow("yuv", edgeMask);
    }
    // Mat bigImg = Mat(size, CV_8U);
    resize(smallImg, srcColor, size, 0, 0, INTER_LINEAR);
    memset((char*)dst.data, 0, dst.step * dst.rows);
    // dst.setTo(0);
    srcColor.copyTo(dst, masking);
    imshow("Cartoonified", dst);
    // return dst;
    // return mask_;
}

/* Code referred from Master OpenCV Book */
// void removePepperNoise(Mat &mask)
// {
//     // For simplicity, ignore the top & bottom row border.
//     for (int y=2; y<mask.rows-2; y++) {
//         // Get access to each of the 5 rows near this pixel.
//         uchar *pThis = mask.ptr(y);
//         uchar *pUp1 = mask.ptr(y-1);
//         uchar *pUp2 = mask.ptr(y-2);
//         uchar *pDown1 = mask.ptr(y+1);
//         uchar *pDown2 = mask.ptr(y+2);
//
//         // For simplicity, ignore the left & right row border.
//         pThis += 2;
//         pUp1 += 2;
//         pUp2 += 2;
//         pDown1 += 2;
//         pDown2 += 2;
//         for (int x=2; x<mask.cols-2; x++) {
//             uchar v = *pThis;   // Get the current pixel value (either 0 or 255).
//             // If the current pixel is black, but all the pixels on the 2-pixel-radius-border are white
//             // (ie: it is a small island of black pixels, surrounded by white), then delete that island.
//             if (v == 0) {
//                 bool allAbove = *(pUp2 - 2) && *(pUp2 - 1) && *(pUp2) && *(pUp2 + 1) && *(pUp2 + 2);
//                 bool allLeft = *(pUp1 - 2) && *(pThis - 2) && *(pDown1 - 2);
//                 bool allBelow = *(pDown2 - 2) && *(pDown2 - 1) && *(pDown2) && *(pDown2 + 1) && *(pDown2 + 2);
//                 bool allRight = *(pUp1 + 2) && *(pThis + 2) && *(pDown1 + 2);
//                 bool surroundings = allAbove && allLeft && allBelow && allRight;
//                 if (surroundings == true) {
//                     // Fill the whole 5x5 block as white. Since we know the 5x5 borders
//                     // are already white, just need to fill the 3x3 inner region.
//                     *(pUp1 - 1) = 255;
//                     *(pUp1 + 0) = 255;
//                     *(pUp1 + 1) = 255;
//                     *(pThis - 1) = 255;
//                     *(pThis + 0) = 255;
//                     *(pThis + 1) = 255;
//                     *(pDown1 - 1) = 255;
//                     *(pDown1 + 0) = 255;
//                     *(pDown1 + 1) = 255;
//                 }
//                 // Since we just covered the whole 5x5 block with white, we know the next 2 pixels
//                 // won't be black, so skip the next 2 pixels on the right.
//                 pThis += 2;
//                 pUp1 += 2;
//                 pUp2 += 2;
//                 pDown1 += 2;
//                 pDown2 += 2;
//             }
//             // Move to the next pixel.
//             pThis++;
//             pUp1++;
//             pUp2++;
//             pDown1++;
//             pDown2++;
//         }
//     }
// }

int main(int argc, char** argv)
{
    bool bDraw = false;
    bool isDragging = false;

    // Read image from file
    img = imread("demo.jpg");

    // initialize your temp images
    layer = img.clone();
    working = img.clone();

    int cartoonIF = 1;
    int threshIF = 0;

    //if fail to read the image
    if( img.empty() )
    {
        cout << "Error loading the image" << endl;
        return -1;
    }

    //Create a window
    namedWindow("My Window", 1);

    //set the callback function for any mouse event
    setMouseCallback("My Window", CallBackFunc, NULL);

    //show the image
    imshow("My Window", working);

    // Wait until user presses 'q'
    while((waitKey(1) & 0xFF) != 'q');

    return 0;
}
