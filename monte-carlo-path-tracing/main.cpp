#include <iostream>
#include "load_obj.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>
#include <vector>
#include "Scene.h"
using namespace std;
using namespace cv;
// image size
#define IMAGE_WIDTH	 512
#define IMAGE_HEIGHT 512


int main()
{	
	// Initial Scene
	Scene scene;

	scene.LoadObjs("./dataset/Scene01/cup.obj");
	scene.AddCamera(Camera(V3(0.0, 0.64, 0.52),V3(0.0, 0.40, 0.3),
						   V3(0.0, 1.0, 0.0),50,IMAGE_WIDTH,IMAGE_HEIGHT));
	scene.AddSphereLight(SphereLight(V3(0.0, 1.589, -1.274),0.2,V3(50, 50, 40)));


	// Rendering
	Mat rst=scene.Rendering();

	// Display result
	imshow("Rendering Result", rst);
	waitKey(0);

	return 0;
}
