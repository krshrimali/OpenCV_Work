#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>

#include <iostream>
#include <sstream>

using namespace cv;
using namespace std;

// Global variables
Mat frame;
Mat fgMaskMOG2; // foreground mask generated by MOG2 method
Ptr<BackgroundSubtractor> pMOG2; // MOG2 Background Subtractor

void help();
void processVideo(char* videoFileName);
void processImages(char* firstframeFilename);

void help(){
	cout
	<< "--------------------------------------------------------------------------" << endl
	<< " OpenCV. You can process both videos (-vid) and images (-img)."             << endl
    << "Usage:"                                                                     << endl
    << "./bg_sub {-vid <video filename>|-img <image filename>}"                     << endl
    << "for example: ./bg_sub -vid video.avi"                                       << endl
    << "or: ./bg_sub -img /data/images/1.png"                                       << endl
    << "--------------------------------------------------------------------------" << endl
    << endl;
}

int main(int argc, char* argv[]){
	help();

	if(argc != 3) {
		cerr << "Incorrect input list" << endl;
		cerr << "Exiting..." << endl;
		return EXIT_FAILURE;
	}

	// create GUI Windows
	namedWindow("Frame");
	namedWindow("FG mask MOG 2");

	// create background subtractor objects
	pMOG2 = createBackgroundSubtractorMOG2(); // MOG2 Approach

	if(strcmp(argv[1], "-vid") == 0) {
		// input data coming from video
		processVideo(argv[2]);
	}
	else if(strcmp(argv[1], "-img") == 0) {
		processImages(argv[2]);
	}
	else {
		// error in reading input parameters
		cerr << "Please check the parameters" << endl;
		cerr << "Exiting" << endl;
		return EXIT_FAILURE;
	}
	// destroy GUI Windows
	destroyAllWindows();
	return EXIT_SUCCESS;
}

void processVideo(char* videoFileName){
	// create capture object
	VideoCapture.capture(videoFileName);

	if(!capture.isOpened()){
		// error in opening video input
		cerr << "Unable to open video file name: " << videoFileName << endl;
		exit(EXIT_FAILURE);
	}
	// read input data, ESC for quitting
	keyboard = 0;
	while(keyboard != 'q' && keyboard != 27){
		// read the current frame
		if(!capture.read(frame)){
			cerr << "Unable to read frame " << endl;
			cerr << "Exiting..." << endl;
			exit(EXIT_FAILURE);
		}
		// update background model
		pMOG2->apply(frame, fgMaskMOG2);
		// get frame number and write it on current frame
		stringstream ss;
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20), cv::Scalar(255, 255, 255), -1);
		ss << capture.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();

		putText(frame, frameNumberString.c_str(), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		
		// show the current frame
		imshow("Frame", frame);
		imshow("FG Mask MOG2", fgMaskMOG2);
		// get the input from keyboard
		keyboard = (char)waitKey(30);		
	}
	// delete capture object
	capture.release();
}

void processImages(char* firstframeFilename) {
	// read first file of the sequence
	frame = imread(firstframeFilename);
	if(frame.empty()) {
		cerr << "Unable to open file: " << firstframeFilename << endl;
		exit(EXIT_FAILURE);
	}
	// current image filename
	string fn(firstframeFilename);
	// read input data
	keyboard = 0;
	while(keyboard != 'q' && keyboard != 27) {
		// update background model
		pMOG2->apply(frame, fgMaskMOG2);
		// get frame number and write it on current frame
		size_t index = fn.find_last_of("/");
		if(index == string::npos) {
			index = fn.find_last_of("\\");
		}
		size_t index2 = fn.find_last_of(".");
		string prefix = fn.substr(0, index+1);
		string suffix = fn.substr(index2);
		string frameNumberString = fn.substr(index+1, index2-index-1);
		istringsstream iss(frameNumberString);
		int frameNumber = 0;
		iss >> frameNumber;

		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20), cv::Scalar(255, 255, 255), -1);
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15), FONT_HERSHEY_SIMPLEX,
			0.5, cv::Scalar(0, 0, 0));
		// show the current mask
		imshow("Frame", frame);
		imshow("Fg mask MOG 2", fgMaskMOG2);
		// get input from keyboard
		keyboard = (char)waitKey(30);
		// search for next image in the sequence
		ostringstream oss;
		oss << (frameNumber + 1);
		string nextFrameNumberString = oss.str();
		string nextFrameFileName = prefix + nextFrameNumberString + suffix;
		// read next frame
		frame = imread(nextFrameFileName);
		if(frame.empty()){
			cerr << "Unable to open next frame: " << nextFrameFileName << endl;
			exit(EXIT_FAILURE);
		}		
		fn.assign(nextFrameFileName);
	}
}