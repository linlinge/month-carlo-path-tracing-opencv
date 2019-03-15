#pragma once
#include "V3.hpp"
#include <vector>
#include "Ray.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>
#include "LoadObjFile.h"
#include "Intersection.hpp"
#include "Camera.hpp"
#include "global.h"
#include "Objects.hpp"
using namespace std;
using namespace cv;

class Scene
{
public:
	Camera camera_;
	ObjFile obj_file_;
	vector<Object*> objs_;
	KdTree tree_;

	Scene(){};
	void LoadObjs(string filename);
	void BuildKdTree();
	void AddCamera(Camera camera);
	void AddSphereLight(SphereLight dat);
	void AddQuadLight(QuadLight dat);

	Mat Rendering();
	V3 RayTracing(Ray& exit_light);
	V3 BlinnPhong(Ray& exit_light,int depth);
	V3 Lambertian(Ray& exit_light,int depth);
	Intersection GetIntersect(Ray& ray); 
};