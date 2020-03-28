#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int, char**)
{
    VideoCapture cap(0); // open the default camera
    if(!cap.isOpened())  // check if we succeeded
    {    
		cout << "camera failed" << endl;
        return -1;
	}


    Mat video;
    namedWindow("video",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, video, COLOR_BGR2HSV);				//convert to hsv
        GaussianBlur(video, video, Size(7,7), 1.5, 1.5);		//smooth out the image
                
        /* 
	// Set threshold and maxValue
	//double thresh = 90;
	//double maxValue = 255; 
 
	*/
	
	
	inRange(video, Scalar(40, 100, 100), Scalar(80, 230, 230), video);  //adjust for green colour of cone
	
	morphologyEx(video, video, MORPH_OPEN, 11);			//adjust morph technique for best results	
	
        imshow("video",video);
        if(waitKey(16) >= 0){}
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
