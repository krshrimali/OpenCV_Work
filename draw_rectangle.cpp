// Source: https://stackoverflow.com/questions/31370459/how-can-i-get-the-position-and-draw-rectangle-using-opencv/31376558#31376558

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

vector<Rect> rects;
bool bDraw;
Rect r; // declare rectangle vector r
Point base;

Mat3b img;
Mat3b layer;
Mat3b working;


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
    if ( event == EVENT_LBUTTONDOWN )
    {
        cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;

        // Init your rect
        base.x = x;
        base.y = y;
        r.x = x;
        r.y = y;
        r.width = 0;
        r.height = 0;
        cout << "Vector: " << r << endl;
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
        cout << "Vector: " << r << endl;
        rectangle(working, r, Scalar(0,255,0));
        imshow("My Window", working);
    }
    else if ( event == EVENT_LBUTTONUP)
    {
        cout << "Left button released" << endl;

        // Save rect, draw it on layer
        rects.push_back(r);
        rectangle(layer, r, Scalar(0,255,255));

        r = Rect();
        bDraw = false;

        // Refresh
        working = layer.clone();
        cout << "Vector: " << r << endl;
        rectangle(working, r, Scalar(0,255,0));
        imshow("My Window", working);
    }
}

int main(int argc, char** argv)
{
    bool bDraw = false;
    bool isDragging = false;

    // Read image from file
    img = imread("demo.jpg");

    // initialize your temp images
    layer = img.clone();
    working = img.clone();

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
