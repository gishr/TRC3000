#include <cv.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char** argv )
{
 char* imageName = argv[1];

 Mat image;
 image = imread( imageName, 1 );

 if( argc != 2 || !image.data )
 {
   printf( " No image data \n " );
   return -1;
 }

 namedWindow( imageName, CV_WINDOW_AUTOSIZE );

 imshow( imageName, image );

 waitKey(0);

 return 0;
}
