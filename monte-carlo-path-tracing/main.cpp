#include <iostream>
#include "load_obj.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>
#include <vector>
using namespace std;
using namespace cv;


// window
#define WINDOW_WIDTH	512
#define WINDOW_HEIGHT	512

// light
float light_position[] = { -2.758771896,1.5246,0 };
float light_normal[] = { 1,0,0 };
float light_emission[] = { 40,40,40 };

// camera
float camera_position[] = { 0.0, 0.64, 0.52 };
float camera_lookat[] = { 0.0, 0.40, 0.3 };
float camera_up[] = { 0.0,1.0,0.0 };

int main()
{
	// load obj file
	Objs objs1;
	objs1.Load("./dataset/Scene01/cup.obj");




	Mat img = imread("./image/1.jpg");
	imshow("wokaka", img);
	waitKey(0);

	return 0;
}
