#pragma once
#include "V3.hpp"
#include <vector>
#include "Ray.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv.h>
#include "load_obj.h"
#include "Intersection.hpp"
using namespace std;
using namespace cv;
#define PI 3.1415926535897

extern vector<Material> mtls_;
class Camera
{    
public:
	V3 position_;
	V3 lookat_;
	V3 applied_up_;
	V3 actual_up_;
	V3 forward_;
	V3 left_;

	float fovy_degree_; // represent with degree
	float fovy_arc_;
	float focal_length_;
	int image_pixel_width_, image_pixel_height_;
	float image_actual_width_, image_actual_height_;
	V3 image_origin_;
	float pixel_width_, pixel_height_;
	Camera(V3 pos,V3 lookat,V3 up,float fovy_degree, float focal_length,int width,int height)
	{
		position_ = pos;
		lookat_ = lookat;
		applied_up_ = up;
		fovy_degree_ = fovy_degree;
		fovy_arc_ = fovy_degree * PI / 180.0;
		focal_length_ = focal_length;
		image_pixel_width_ = width;
		image_pixel_height_ = height;

		// actual up, left and forward diretion
		float arc = GetArc(position_ - lookat_, applied_up_);
		actual_up_ = (applied_up_*sin(arc)).GetNorm();		
		forward_ = (lookat_ - position_).GetNorm();
		left_ = (Cross(forward_, actual_up_)).GetNorm();

		// image origin, pixel height, pixel width
		float r = tan(fovy_arc_ / 2.0f)*focal_length_*2.0f;
		float alpha = atan(1.0f*image_pixel_width_ / image_pixel_height_);
		image_actual_width_ = r * cos(alpha);
		image_actual_height_ = r * sin(alpha);
		pixel_width_ = image_actual_width_ * 1.0f / image_pixel_width_;
		pixel_height_ = image_actual_height_ * 1.0f / image_pixel_height_;

		image_origin_ = position_ + focal_length_ * forward_ + image_actual_width_ / 2.0f*left_ + image_actual_height_ / 2.0f*actual_up_;
	}

	Camera() {};
	Camera operator=(Camera& dat)
	{
		position_ = dat.position_;
		lookat_ = dat.lookat_;
		applied_up_ = dat.applied_up_;
		actual_up_ = dat.actual_up_;		
		fovy_degree_ = dat.fovy_degree_;
		fovy_arc_ = dat.fovy_arc_;
		image_pixel_width_ = dat.image_pixel_width_;
		image_pixel_height_ = dat.image_pixel_height_;
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
		V3 hypotenuse = center_ - ray.origin_;
		float arc = acos(Dot(ray.direction_.GetNorm(), hypotenuse.GetNorm()));
		float dist = Distance(ray.origin_, center_)*sin(arc);

		if (dist < radius_)
		{
			itsc.is_hit_ = true;
		}
		else
		{
			itsc.is_hit_ = false;

		}
		

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
	V3 IlluminationModel(Ray& ray,Intersection itsc);
};