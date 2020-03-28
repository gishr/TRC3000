#include "opencv2/opencv.hpp"
#include <iostream>
#include <ctime>
#include <raspicam/raspicam_cv.h>
#include <highgui.h>

using namespace cv;
using namespace std;

int main(int, char**)
{
	raspicam::RaspiCam_Cv Camera;
	Mat image, output;
	Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1);
	cout<< "Opening camera ..." <<endl;
	if (!Camera.open())
	{
		cerr<< "Error opening the camera" <<endl;
		return -1;
	}
	cout << "Capturing" <<endl;
	while (1)
	{
		Camera.grab();
		Camera.retrieve(output);
		imshow("stream",output);
		char c =(char) waitKey(10000);
		if(c==27) {break;}
	}
	cout <<"Stop camera..." << endl;
	Camera.release();
	
    return 0;
}
