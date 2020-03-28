#include "opencv2/opencv.hpp"
#include <iostream>
#include "blob.h"

using namespace cv;
using namespace std;

void blobs(int &largest_area) 
{
    
    VideoCapture cap(0); // open the default camera
    /*
    if(!cap.isOpened())  // check if we succeeded
    {    
		cout << "camera failed" << endl;
	}
	*/
	//Rect bounding_rect;
	Scalar color = Scalar(0, 255, 0);

    Mat video;
    namedWindow("Output",1);
    for(;;)
    {
        Mat frame;
        cap >> frame; // get a new frame from camera
		flip(frame,frame,0);						//flip image because cam upside down
        cvtColor(frame, video, COLOR_BGR2HSV);				//convert to hsv
        GaussianBlur(video, video, Size(7,7), 1.5, 1.5);		//smooth out the image   
    	
		//inRange(video, Scalar(50, 100, 100), Scalar(80, 230, 230), video);  //adjust for green colour of cone
		inRange(video, Scalar(40, 40, 70), Scalar(90, 230, 230), video);  //for debugging
		
		
		morphologyEx(video, video, MORPH_OPEN, 11);			//adjust morph technique for best results	
		morphologyEx(video, video, MORPH_CLOSE, 11);
		
		/*
		Moments m = moments(video,true);					//find centroid of cone
		Point p(m.m10/m.m00, m.m01/m.m00);
		circle(video, p, 5, Scalar(128,0,0), -1);
		*/
		
		// Drawing Contours
		vector<vector<Point> > contours; // Vector for storing contours
		vector<Vec4i> hierarchy;
		
		findContours(video,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE); //find contours
		
		int largest_area=0;
		int largest_contour_index=0;
				
		for( size_t i = 0; i< contours.size(); i++ ) // iterate through each contour.
		{
			double area = contourArea( contours[i] );  //  Find the area of contour
			
		    if( area > largest_area )
		    {
				largest_area = area;
				largest_contour_index = i;               //Store the index of largest contour
				//bounding_rect = boundingRect( contours[i] ); // Find the bounding rectangle for biggest contour
			}
		    
		   
		    //drawContours( video, contours, largest_contour_index, color, -1, LINE_8, hierarchy, 0 );
		}
		
		cout<<"area =  "<<largest_area<<endl;		//debugging
		
        imshow("Output",video);
        if(waitKey(16) >= 0){}
    }
    // the camera will be deinitialized automatically in VideoCapture destructor
    
}
