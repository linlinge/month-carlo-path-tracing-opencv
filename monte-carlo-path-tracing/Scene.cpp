#include "Scene.h"


void Scene::AddCamera(Camera camera)
{
	camera_ = camera;
}

void Scene::AddSphereLight(SphereLight dat)
{
	sphere_light_.push_back(dat);
}

void Scene::AddQuadLight(QuadLight dat)
{
	quad_light_.push_back(dat);
}

Mat Scene::Rendering()
{
	// Define
	Mat result(camera_.image_pixel_width_,camera_.image_pixel_height_,CV_8UC3,cv::Scalar(0,0,0));
	/*imshow("hi", result);
	waitKey(0);*/



	// generate rays in each pixel	
	for (int i = 0; i < camera_.image_pixel_width_; i++)
	{
		for (int j = 0; j < camera_.image_pixel_height_; j++)
		{
			// Define
			V3 color;
			// Generate camera/primary ray					
			V3 pos = V3(camera_.image_origin_.x + camera_.pixel_width_ / 2.0f*i, camera_.image_origin_.y + camera_.pixel_width_ / 2.0f*j, camera_.image_origin_.z);						
			Ray ray;
			ray.origin_ = camera_.position_;
			ray.direction_ = (pos - camera_.position_).GetNorm();

			// Ray Tracing
			color=RayTracing(ray);
			color = color * 255;
			result.at<Vec3b>(i, j) = Vec3b(color.b,color.g,color.r);
		}
	}
	return result;
}

// trace one ray 
V3 Scene::RayTracing(Ray& ray)
{
	V3 color;
	// Get Nearest Patch
	Intersection itsc=tree.NearestSearchByLevel(ray);	
	if (itsc.is_hit_ == true)
	{
		color=IlluminationModel(ray, itsc);
	}	



	return color;
}

V3 Scene::IlluminationModel(Ray& ray, Intersection itsc)
{
	// specular light: use blinn-phong model
	V3 light_direction = (sphere_light_[0].center_ - itsc.point_).GetNorm();
	V3 view_direction = (camera_.position_ - itsc.point_).GetNorm();
	V3 halfway_direction = (light_direction + view_direction).GetNorm();


	Material& mtl_temp = mtls_[itsc.mtl_id_];
	V3 Rs = mtl_temp.Ks_*light_direction*pow(MAX2(0,Dot(itsc.normal_,halfway_direction)), mtl_temp.Ns_);
	
	return Rs;
	// diffuss light: 


}

void Scene::LoadObjs(string filename)
{
	objs_.LoadObjs(filename);
}
