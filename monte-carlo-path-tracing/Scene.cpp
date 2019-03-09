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
	Mat result(camera_.image_width_,camera_.image_height_,CV_8UC3,cv::Scalar(0,0,0));
	/*imshow("hi", result);
	waitKey(0);*/

	// generate rays in each pixel
	float ratio = camera_.image_width_ * 1.0 / camera_.image_height_;
	for (int i = 0; i < camera_.image_width_; i++)
	{
		for (int j = 0; j < camera_.image_height_; j++)
		{
			// Define
			V3 color;

			// Generate camera ray			
			float Px = (2 * (i + 0.5) / camera_.image_width_ - 1)*ratio*tan(camera_.fovy_arc_ / 2.0);
			float Py = (1 - 2 * (j + 0.5)) / tan(camera_.fovy_arc_);
			Ray ray;
			ray.origin_ = camera_.position_;
			ray.direction_ = (V3(Px, Py, -1) - camera_.position_).GetNorm();

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
	Patch nearest_patch;
	V3 intersection;
	bool temp=tree.NearestSearchByLevel(ray,nearest_patch, intersection);	
	if (temp == true)
	{
		color=IlluminationModel(ray, nearest_patch, intersection);
	}	



	return color;
}

V3 Scene::IlluminationModel(Ray& ray,Patch& patch,V3& intersection)
{
	// specular light: use blinn-phong model
	V3 light_direction = (sphere_light_[0].center_ - intersection).GetNorm();
	V3 view_direction = (camera_.position_ - intersection).GetNorm();
	V3 halfway_direction = (light_direction + view_direction).GetNorm();


	Material& mtl_temp = mtls_[patch.mtl_id_];
	V3 Rs = mtl_temp.Ks_*light_direction*pow(MAX2(0,Dot(patch.normal_,halfway_direction)), mtl_temp.Ns_);
	
	return Rs;
	// diffuss light: 


}

void Scene::LoadObjs(string filename)
{
	objs_.LoadObjs(filename);
}
