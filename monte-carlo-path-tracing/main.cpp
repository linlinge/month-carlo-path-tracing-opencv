#include <iostream>
#include "LoadObjFile.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>
#include <vector>
#include "Scene.h"
#include <ostream>
using namespace std;
using namespace cv;
// image size
#define IMAGE_WIDTH	500
#define IMAGE_HEIGHT 500


int main()
{	
	// Initial Scene
	Scene scene;
	// load obj file
	//scene.LoadObjs("./dataset/Example01/example01.obj");
	//scene.LoadObjs("./dataset/Example02/example02.obj");
	//scene.LoadObjs("./dataset/Scene01/cup.obj");
	scene.LoadObjs("./dataset/Scene02/room.obj");
	
	// add objects
	//scene.AddCamera(Camera(V3(0.0, 0.0, 0.4),V3(0.0, 0.0, 0.0),V3(0.0,1.0,0.0),
	//				50,10.0f,IMAGE_WIDTH,IMAGE_HEIGHT));
	scene.AddCamera(Camera(V3(1.0, 2, 1), V3(-5.09809,-5.6217,-4.08914), V3(1.0, 0.0, 0.0),
		100, 1.0f, IMAGE_WIDTH, IMAGE_HEIGHT));

	/*scene.AddCamera(Camera(V3(0,0,6), V3(-5.72581, -3.16399, 0), V3(0.0, 1.0, 0.0),
		110, 10.0f, IMAGE_WIDTH, IMAGE_HEIGHT));*/

	scene.AddSphereLight(SphereLight(V3(1.0, 10,10),0.2,V3(50, 50, 40)));

	// establish kd-tree
	scene.BuildKdTree();

	Mat temp = imread("./image/1.jpg");
	// Rendering
	Mat rst=scene.Rendering();

	global_file.close();
	//tree.Print();
	// Display result
	imshow("Rendering Result", rst);
	waitKey(0);


	//// test for intersection between AABB box and ray
	//AABB box_temp(V3(0,0,0),V3(1,1,1));
	//Ray ray;
	//ray.origin_ = V3(0.5, 0.5, 0.5);
	//ray.direction_ = V3(0.5,0.5, -3.0);
	//cout << box_temp.IsIntersect(ray) << endl;
	//system("pause");
	
	
	//// test for intersection between polygon and ray
	//Patch& p = scene.objs_.f_[500];	
	//ofstream file("../output/patch.txt");
	//Ray ray;
	//ray.origin_ = V3(0.07, -0.16, 0.01);
	//ray.direction_ = V3(-0.4200,-0.7400,-0.1800);
	//file << ray.origin_.x << " " << ray.origin_.y << " " << ray.origin_.z << endl;
	//file << ray.direction_.x << " " << ray.direction_.y << " " << ray.direction_.z << endl;

	//for (int i = 0; i < p.vn_id_.size(); i++)
	//{
	//	file << v_[p.v_id_[i]].x << " " << v_[p.v_id_[i]].y << " " << v_[p.v_id_[i]].z<<endl;
	//}
	//file.close();
	//
	//cout << p.IsIntersect(ray) << endl;
	//system("pause");

	return 0;
}
