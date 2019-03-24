#include "Scene.h"

void Scene::AddCamera(Camera camera)
{
	camera_ = camera;
	// resize buffer
	buffer_.resize(camera_.image_pixel_height_*camera_.image_pixel_width_);
}

void Scene::AddSphereLight(SphereLight dat)
{	
	objs_.push_back(&dat);
	sphere_light_.push_back(dat);
}

void Scene::AddQuadLight(QuadLight dat)
{	
	objs_.push_back(&dat);
	quad_light_.push_back(dat);
}

void Scene::BuildKdTree()
{
	 tree_.Build(objs_, 0);
}

Mat Scene::Rendering()
{
	// Define
	Mat result(camera_.image_pixel_width_,camera_.image_pixel_height_,CV_8UC3,cv::Scalar(0,0,0));

	//ofstream f("../matlab/ray.txt");

	// Generate rays in each pixel
	float max_val = 0;
	float min_val = INT_MAX;
	for (int i = 0; i < camera_.image_pixel_width_; i++)
	{
		for (int j = 0; j < camera_.image_pixel_height_; j++)
		{
			// Define
			V3 color;
			
			//for (int k = 0; k < 10; k++)
			//{
			//	//srand((unsigned)time(NULL));
			//	float rdm1 = (rand() / double(RAND_MAX)-0.5);
			//	//srand((unsigned)time(NULL));
			//	float rdm2 = (rand() / double(RAND_MAX)-0.5);				
			//	// Generate camera/primary ray
			//	V3 pos = camera_.GetPosition(i+rdm1, j+rdm2);
			//	//V3 pos= camera_.GetPosition(i, j);
			//	Ray ray;
			//	ray.origin_ = camera_.position_;
			//	ray.direction_ = (pos - camera_.position_).GetNorm();

			//	// f << ray.origin_ <<" "<<pos<< endl;
			//	// Ray Tracing
			//	color=color+RayTracing(ray);
			// }
			// color = color / 10.0f;


			// not jitter
			V3 pos = camera_.GetPosition(i, j);
			Ray ray;
			ray.origin_ = camera_.position_;
			ray.direction_ = (pos - camera_.position_).GetNorm();
			if (i == 330 && j == 160)
			{
				i = 330;
			}
			color = RayTracing(ray);

			min_val= min(color.x, min(color.y, min(color.z, min_val)));
			max_val = max(color.x, max(color.y, max(color.z, max_val)));
			//result.at<Vec3b>(i, j) = Vec3b(color.b,color.g,color.r);
			buffer_[i*camera_.image_pixel_width_ + j] = color;
			
			//cout << color.x << " ";
		}
		//cout << endl;
	}
	//f.close();
		
	float delta = max_val - min_val;

	for (int i = 0; i < camera_.image_pixel_width_; i++)
	{
		for (int j = 0; j < camera_.image_pixel_height_; j++)
		{
			result.at<Vec3b>(i, j)[0] =(buffer_[i*camera_.image_pixel_width_+j].x-min_val) /delta *255;
			result.at<Vec3b>(i, j)[1] =(buffer_[i*camera_.image_pixel_width_+j].y-min_val) / delta *255;
			result.at<Vec3b>(i, j)[2] =(buffer_[i*camera_.image_pixel_width_+j].z-min_val) / delta *255;

			/*result.at<Vec3b>(i, j)[0] = buffer_[i*camera_.image_pixel_width_ + j].x;
			result.at<Vec3b>(i, j)[1] = buffer_[i*camera_.image_pixel_width_ + j].y;
			result.at<Vec3b>(i, j)[2] = buffer_[i*camera_.image_pixel_width_ + j].z;*/
			/*if (i == 330 || j==160)
			{
				result.at<Vec3b>(i, j)[0] = 255;
				result.at<Vec3b>(i, j)[1] = 0;
				result.at<Vec3b>(i, j)[2] = 0;
			}*/

		}		
	}
	cout << max_val << endl;
	return result;
}

// trace one ray 
V3 Scene::RayTracing(Ray& ray)
{
	V3 color;
	//V3 temp = BlinnPhong(ray, 0);
	color = Lambertian(ray);//  +temp;
	return color;
}

// Difuss model
V3 Scene::Lambertian(Ray& exit_light)
{
	V3 color;
	Ray incident;
	// Get Nearest Patch
	Intersection itsc = tree_.NearestSearchByLevel(exit_light);
	//if (itsc.is_hit_==true && itsc.type_ == PATCH)
	//{		
	//	global_file<< itsc.pPatch_->v_id_[0] << " ";
	//	global_file<< itsc.pPatch_->v_id_[1] << " ";
	//	global_file << itsc.pPatch_->v_id_[2] << endl;
	//}
	if (itsc.is_hit_ == false)
		return background;

	if (itsc.pMtl_ == NULL)
		return background;
	
	// abnormal situation1: hit light source
	if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
	{
		return *itsc.pLe_;
	}

	V3 intersection_to_lightsource = (sphere_light_[0].center_ - itsc.intersection_).GetNorm();
	color = itsc.pMtl_->Kd_*sphere_light_[0].Le_*abs(Dot(itsc.normal_, intersection_to_lightsource));
	return color;
}

// specular model
V3 Scene::BlinnPhong(Ray& exit_ray,int depth)
{
	// Get intersection
	Intersection itsc = tree_.NearestSearchByLevel(exit_ray);

	// Condition
	if (itsc.is_hit_ == false)
		return background;

	if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
		/// Light
	{
		return *itsc.pLe_;
	}

	if (depth > 8)
		/// Out of maximum depth
	{
		return V3(50, 50, 40);
	}

	// Define
	V3 color;
	// Get incident light
	Ray incident_ray = GetIncidentRay(exit_ray, itsc);
	V3 L = incident_ray.direction_;
	V3 V = (camera_.position_ - itsc.intersection_).GetNorm();
	V3 H = (L + V) / (L + V).GetLength();

	// Caculate color
	color = itsc.pMtl_->Ks_*BlinnPhong(incident_ray,depth+1)*pow(MAX2(0,Dot(itsc.normal_,H)), itsc.pMtl_->Ns_);
	return color;
}


void Scene::LoadObjs(string filename)
{
	// load file
	obj_file_.LoadObjs(filename);

	// convert into objects
	for (int i = 0; i < obj_file_.f_.size(); i++)
	{
		objs_.push_back(&obj_file_.f_[i]);
	}
}


Ray& Scene::GetIncidentRay(Ray& exit_light, Intersection& itsc)
{
	// make sure that direction of exit light is nomalized
	exit_light.direction_.Normalize();
	Ray incident_ray;
	incident_ray.origin_ = itsc.intersection_;
	// calculate direction
	float cos_theta = Dot(exit_light.direction_,itsc.normal_);
	cos_theta = -1.0f*cos_theta; // you should make sure that direction is right
	float D = 1.0f*cos_theta; // because the direction vector is a normalized vector
	incident_ray.direction_ = (exit_light.direction_ + 2 * D*itsc.normal_).GetNorm();
	return incident_ray;
}