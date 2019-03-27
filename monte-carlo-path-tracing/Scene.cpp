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
	light_source_.push_back(&dat);
}

void Scene::AddQuadLight(QuadLight& dat)
{	
	objs_.push_back(&dat);	
	light_source_.push_back(&dat);
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
			if (i == 330 && j == 100)
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
			result.at<Vec3b>(i, j)[0] = (buffer_[i*camera_.image_pixel_width_ + j].z - min_val) / delta * 255;
			result.at<Vec3b>(i, j)[1] = (buffer_[i*camera_.image_pixel_width_ + j].y - min_val) / delta * 255;
			result.at<Vec3b>(i, j)[2] = (buffer_[i*camera_.image_pixel_width_ + j].x - min_val) / delta * 255; 

			/*result.at<Vec3b>(i, j)[0] = buffer_[i*camera_.image_pixel_width_ + j].x;
			result.at<Vec3b>(i, j)[1] = buffer_[i*camera_.image_pixel_width_ + j].y;
			result.at<Vec3b>(i, j)[2] = buffer_[i*camera_.image_pixel_width_ + j].z;*/
			/*if (i == 330 || j==100)
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
	V3 temp = BlinnPhong(ray, 0);
	color = Lambertian(ray) +temp;
	return color;
}

// Difuss model
V3 Scene::Lambertian(Ray& exit_light)
{
	V3 color;
	Ray incident;
	// Get Nearest Patch
	Intersection itsc = tree_.NearestSearchByLevel(exit_light);
	
	if (itsc.is_hit_ == false)
		return background;

	if (itsc.pMtl_ == NULL)
		return background;
	
	// abnormal situation1: hit light source
	if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
	{
		return *itsc.pLe_;
	}
	SphereLight* pLight0 = NULL;
	QuadLight* pLight1 = NULL;
	Ray detect_ray;
	Intersection detect_itsc;
	detect_ray.origin_ = itsc.intersection_;
	for (int i = 0; i < light_source_.size(); i++)
	{
		V3 direction_temp = (light_source_[i]->center_ - itsc.intersection_).GetNorm();
		color = color + itsc.pMtl_->Kd_*light_source_[i]->Le_*abs(Dot(itsc.normal_, direction_temp));
	}
		
	//for (auto& object_temp : light_source_)
	//{
	//	switch (object_temp->type_)
	//	{
	//	case SPHERE_SOURCE:
	//		pLight0 = (SphereLight*)object_temp;
	//		// shadow check lines
	//		for (int i = 0; i < NUMBER_OF_LIGHT_SAMPLES; i++)
	//		{
	//			// Get random points from light				
	//			detect_ray.direction_ = (pLight0->Sampling() - itsc.intersection_).GetNorm(); // 整理这里的代码				
	//			detect_itsc = tree_.NearestSearchByLevel(detect_ray);				
	//			/*if (detect_itsc.is_hit_ == true && detect_itsc.type_ == PATCH )
	//				color = V3(0,0,0);*/
	//		}
	//		color = color / NUMBER_OF_LIGHT_SAMPLES;
	//		break;
	//	case QUAD_SOURCE:
	//		pLight1 = (QuadLight*)object_temp;			
	//		detect_ray.direction_ = (pLight1->center_ - itsc.intersection_).GetNorm();			
	//		detect_itsc = tree_.NearestSearchByLevel(detect_ray);
	//		color = color + itsc.pMtl_->Kd_*pLight1->Le_*abs(Dot(itsc.normal_, detect_ray.direction_));
	//		if (detect_itsc.is_hit_ == true && detect_itsc.type_ == PATCH && detect_itsc.distance_ < 0.1f && detect_itsc.distance_ > 0.01f)
	//			color = V3(0,0,0);
	//		break;
	//	}
	//}

	color = color / (float)light_source_.size();
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

	if (depth > 3)
		/// Out of maximum depth
	{
		return V3(1, 1, 1);
	}

	// Define
	V3 color;
	// Get incident light
	Ray incident_ray = GetIncidentRay(exit_ray, itsc);
	V3 L = incident_ray.direction_;
	V3 V = (camera_.position_ - itsc.intersection_).GetNorm();
	V3 H = (L + V) / (L + V).GetLength();

	// Caculate color
	V3 temp1 = BlinnPhong(incident_ray, depth + 1);
	float temp2 = pow(MAX2(0, Dot(itsc.normal_, H)), itsc.pMtl_->Ns_);
	color = itsc.pMtl_->Ks_*temp1*temp2;
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
	Ray incident_ray;
	incident_ray.origin_ = itsc.intersection_;
	// calculate direction
	float cos_theta = Dot(exit_light.direction_,itsc.normal_);
	cos_theta = -1.0f*cos_theta; // you should make sure that direction is right
	float D = 1.0f*cos_theta; // because the direction vector is a normalized vector
	incident_ray.direction_ = (exit_light.direction_ + 2 * D*itsc.normal_).GetNorm();
	return incident_ray;
}