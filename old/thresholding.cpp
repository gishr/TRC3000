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


    Mat edges;
    namedWindow("edges",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
        cvtColor(frame, edges, COLOR_BGR2GRAY);					//convert to bw
        GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);		//filter out noise
        //Canny(edges, edges, 0, 70, 3);						//edge detection
        //canny threshold originally set to 30, trial and error this
        
         
		// Set threshold and maxValue
		double thresh = 90;
		double maxValue = 255; 
 
		// Binary Threshold
		threshold(edges,edges, thresh, maxValue, THRESH_BINARY);
		
				        
        imshow("edges", edges);
        //cout << "code reached for loop" << endl;
        if(waitKey(16) >= 0){}
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
