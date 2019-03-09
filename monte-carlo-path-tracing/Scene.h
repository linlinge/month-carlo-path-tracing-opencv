#pragma once
#include "V3.hpp"
#include <vector>
#include "Ray.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>
#include "load_obj.h"
using namespace std;
using namespace cv;
#define PI 3.1415926535897

extern vector<Material> mtls_;

class Intersection
{
public:
	bool is_hit_;
	V3 point_;
	V3 normal_;
	Ray incident_light_;
};

class Camera
{    
public:
	V3 position_;
	V3 lookat_;
	V3 up_;
	float fovy_degree_; // represent with degree
	float fovy_arc_;
	int image_width_, image_height_;
	Camera(V3 pos,V3 lookat,V3 up,float fovy_degree, int width,int height)
	{
		position_ = pos;
		lookat_ = lookat;
		up_ = up;
		fovy_degree_ = fovy_degree;
		fovy_arc_ = fovy_degree * PI / 180.0;
		image_width_=width;
		image_height_ = height;
	}
	Camera() {};
	Camera operator=(Camera& dat)
	{
		position_ = dat.position_;
		lookat_ = dat.lookat_;
		up_ = dat.lookat_;
		fovy_degree_ = dat.fovy_degree_;
		fovy_arc_ = dat.fovy_arc_;
		image_width_ = dat.image_width_;
		image_height_ = dat.image_height_;
		return *this;
	}
};


class Object
{
public:

	virtual Intersection IsIntersect(Ray& ray)=0;
};
 
class SphereLight:public Object
{
public:
	V3 center_;
	float radius_;
	V3 Le_;
	SphereLight(V3 center, float radius, V3 Le)
	{
		center_ = center;
		radius_ = radius;
		Le_ = Le;
	}

	SphereLight operator=(SphereLight& dat)
	{
		center_ = dat.center_;
		radius_ = dat.radius_;
		Le_ = dat.Le_;
		return *this;
	}

	virtual Intersection IsIntersect(Ray& ray)
	{
		Intersection itsc;
		
		

		return itsc;
	}
};

class QuadLight:public Object
{
public:
	V3 center_;
	V3 normal_;
	float size_[2];
	V3 Le_;

	QuadLight(V3 center,V3 normal,float size[2],V3 Le)
	{
		center_ = center;
		normal_ = normal;
		size_[0] = size[0]; size_[1] = size[1];
		Le_ = Le;
	}

	virtual Intersection IsIntersect(Ray& ray)
	{

		return Intersection();
	}
};

class Scene
{
public:
	Camera camera_;
	Objs objs_;
	vector<SphereLight> sphere_light_;
	vector<QuadLight> quad_light_;


	Scene(){};
	void LoadObjs(string filename);
	void AddCamera(Camera camera);
	void AddSphereLight(SphereLight dat);
	void AddQuadLight(QuadLight dat);

	Mat Rendering();
	V3 RayTracing(Ray& ray);
	V3 IlluminationModel(Ray& ray, Patch& patch,V3& intersection);
};