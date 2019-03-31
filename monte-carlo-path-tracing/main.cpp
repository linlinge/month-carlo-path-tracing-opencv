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


int main()
{	
	// Initial Scene
	Scene scene;
	
	// example01
	/*scene.LoadObjs("./dataset/Example01/example01.obj");
	scene.AddCamera(Camera(V3(4.0, 1, 1), V3(-5.09809, -5.6217, 4.08914), V3(1.0, 0.0, 0.0),
		130, 1.0f, IMAGE_WIDTH, IMAGE_HEIGHT));
	scene.AddSphereLight(SphereLight(V3(5.0, 4.589, -4.274), 0.2, V3(30, 30, 30)));*/

	// example02
	//scene.LoadObjs("./dataset/Example02/example02.obj");
	//scene.AddCamera(Camera(V3(4.0, 4, 4), V3(0,0,0.5), V3(0.0, 0.0, 1.0),
	//	50, 10.0f, IMAGE_WIDTH, IMAGE_HEIGHT));
	//scene.AddSphereLight(SphereLight(V3(0, 0, 5), 0.2, V3(50, 50, 50)));
	
	// example03
	/*scene.LoadObjs("./dataset/Example03/example03.obj");
	scene.AddCamera(Camera(V3(1.0, 2, 1), V3(-5.09809, -5.6217, 4.08914), V3(1.0, 0.0, 0.0),
		130, 1.0f, IMAGE_WIDTH, IMAGE_HEIGHT));
	scene.AddSphereLight(SphereLight(V3(1.0, 10, 10), 0.2, V3(50, 50, 40)));*/

	// example04
	/*scene.LoadObjs("./dataset/Example04/example04.obj");
	scene.AddCamera(Camera(V3(1.0, 3, 1), V3(1,0,0), V3(0.0, 0.0, 1.0),
		160, 10.0f, IMAGE_WIDTH, IMAGE_HEIGHT));
	scene.AddSphereLight(SphereLight(V3(-6.4301, 8.45732, 6), 1.41, V3(180, 180, 180)));*/

	// room
	/*scene.LoadObjs("./dataset/Scene01/room.obj");
	scene.AddCamera(Camera(V3(0.0, 0.0, 4), V3(0.0, 0.0, 0.0), V3(0.0,1.0,0.0),
		80, 10.0f, IMAGE_WIDTH, IMAGE_HEIGHT));
	scene.AddSphereLight(SphereLight(V3(0.0, 1.589, -1.274),0.2,V3(50, 50, 40)));*/

	// cup
	scene.LoadObjs("./dataset/Scene02/cup.obj");
	scene.AddCamera(Camera(V3(0.0, 0.64, 0.52), V3(0.0, 0.40, 0.3), V3(0.0, 1.0, 0.0),
		80, 1.0f, IMAGE_WIDTH, IMAGE_HEIGHT));
	QuadLight quad_temp = QuadLight(V3(-2.758771896, 1.5246, 0), V3(1, 0, 0), V2(1,1), V3(40,40,40));
	scene.AddQuadLight(quad_temp);
	
	// Veach MIS
	/*scene.LoadObjs("./dataset/Scene03/VeachMIS.obj");
	scene.AddCamera(Camera(V3(0.0, 2.0, 15.0), V3(0.0, 1.69521, 14.0476), V3(0.0, 0.952421, -0.304787),
		50, 10.0f, IMAGE_WIDTH, IMAGE_HEIGHT));
	scene.AddSphereLight(SphereLight(V3(-10, 10, 4), 0.5, V3(800, 800, 800)));
	scene.AddSphereLight(SphereLight(V3(3.75, 0, 0), 0.033, V3(901.803, 901.803, 901.803)));
	scene.AddSphereLight(SphereLight(V3(1.25, 0, 0), 0.1, V3(100, 100, 100)));
	scene.AddSphereLight(SphereLight(V3(-1.25, 0, 0), 0.3, V3(11.1111, 11.1111, 11.1111)));
	scene.AddSphereLight(SphereLight(V3(-3.75, 0, 0), 0.9, V3(1.23457, 1.23457, 1.23457)));*/

	// establish kd-tree
	scene.BuildKdTree();
	Mat temp = imread("./image/1.jpg");
	// Rendering
	Mat rst=scene.Rendering();

	// Display result
	imshow("Rendering Result", rst);
	waitKey(0);

	/*for(int i=0;i<10;i++)
		cout << GetRandomFloat() << endl;
	system("pause");*/

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