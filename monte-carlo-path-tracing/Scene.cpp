#include "Scene.h"

int temp_count1 = 0;
int temp_count2 = 0;
int temp_count3 = 0;
void Scene::AddCamera(Camera camera)
{
	camera_ = camera;
	// resize buffer
	buffer_.resize(camera_.image_pixel_height_*camera_.image_pixel_width_);
}

void Scene::AddSphereLight(SphereLight dat)
{	
	objs_.push_back(&dat);
	light_.push_back(&dat);
}

void Scene::AddQuadLight(QuadLight& dat)
{	
	objs_.push_back(&dat);	
	light_.push_back(&dat);
}

void Scene::BuildKdTree()
{
	 tree_.Build(objs_, 0);
}

Mat Scene::Rendering()
{
	// Define
	Mat result(camera_.image_pixel_width_,camera_.image_pixel_height_,CV_8UC3,cv::Scalar(0,0,0));
	// Generate rays in each pixel
	float max_val = 0;
	float min_val = INT_MAX;
	for (int i = 0; i < camera_.image_pixel_width_; i++)
	{
		for (int j = 0; j < camera_.image_pixel_height_; j++)
		{
			// Define
			V3 color;
			V3 pos = camera_.GetPosition(i, j);
			Ray ray(camera_.position_,(pos-camera_.position_).GetNorm());

			// ray tracing
			color = RayTracing(ray);
			// store color
			min_val= min(color.x, min(color.y, min(color.z, min_val)));
			max_val = max(color.x, max(color.y, max(color.z, max_val)));			
			buffer_[i*camera_.image_pixel_width_ + j] = color;						
		}
	}

	// fill Mat 
	float delta = max_val - min_val;
	for (int i = 0; i < camera_.image_pixel_width_; i++)
	{
		for (int j = 0; j < camera_.image_pixel_height_; j++)
		{
			result.at<Vec3b>(i, j)[0] = (buffer_[i*camera_.image_pixel_width_ + j].z - min_val) / delta * 255;
			result.at<Vec3b>(i, j)[1] = (buffer_[i*camera_.image_pixel_width_ + j].y - min_val) / delta * 255;
			result.at<Vec3b>(i, j)[2] = (buffer_[i*camera_.image_pixel_width_ + j].x - min_val) / delta * 255; 
		}		
	}

	// output result
	cout << max_val << endl;
	return result;
}

// trace one ray 
V3 Scene::RayTracing(Ray& ray)
{
	Intersection itsc = tree_.NearestSearchByLevel(ray);
	float scale=ShadowTest(itsc.intersection_);
	V3 color = scale * (Lambertian(itsc));// +BlinnPhong(ray, 0));
	return color;
}

// Shadow Test
float Scene::ShadowTest(V3& intersection)
{
	SphereLight* pSphereLight=NULL;
	QuadLight* pQuadLight = NULL;
	float scale = 0.0f;
	for (int i=0;i< light_.size();i++)
	{
		switch (light_[i]->type_)
		{
		case SPHERE_SOURCE:
			pSphereLight = (SphereLight*)&light_[i];
			// shadow check lines
			for (int i = 0; i < NUMBER_OF_LIGHT_SAMPLES; i++)
			{
				// Get random points from light
				Ray shadow_ray(intersection, (pSphereLight->Sampling() - intersection).GetNorm());				
				Intersection shadow_itsc = tree_.NearestSearchByLevel(shadow_ray);				
				if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == PATCH && shadow_itsc.distance_ < 0.1)// && shadow_itsc.distance_<0.1f && shadow_itsc.distance_ > 0.02f)
				{
					scale = scale+1;
				}
			}		
			break;

		case QUAD_SOURCE:
		/*	pQuadLight = (QuadLight*)&light_[i];			
			detect_itsc = tree_.NearestSearchByLevel(detect_ray);
			color = color + itsc.pMtl_->Kd_*pLight1->Le_*abs(Dot(itsc.normal_, detect_ray.direction_));
			if (detect_itsc.is_hit_ == true && detect_itsc.type_ == PATCH && detect_itsc.distance_ < 0.1f && detect_itsc.distance_ > 0.01f)
				color = V3(0,0,0);*/
			break;
		}
	}
	if(scale <0.5)
		scale =1-scale / (NUMBER_OF_LIGHT_SAMPLES*1.0f);
	

	return scale;
}

// Difuss model
V3 Scene::Lambertian(Intersection& itsc)
{
	V3 color;	
	if (itsc.is_hit_ == false)
		/// not hit 
	{
		return background;
	}		
	else if (itsc.pMtl_ == NULL)
		/// no material
	{
		cout << "Error: no coresponding material file" << endl;
	}		
	else if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
		/// abnormal situation1: hit light source
	{		
		return *itsc.pLe_;
	}

	//itsc.intersection_ = itsc.intersection_ + V3(0.001, 0, 0);
	// diffuse light
	for (int i = 0; i < light_.size(); i++)
	{
		V3 direction_temp = (light_[i]->center_ - itsc.intersection_).GetNorm();
		color = color + itsc.pMtl_->Kd_*light_[i]->Le_*abs(Dot(itsc.normal_, direction_temp));
	}	
	color = color / (float)light_.size();
	return color;
}

// specular model
V3 Scene::BlinnPhong(Ray& exit_ray,int depth)
{
	// Get intersection
	Intersection itsc = tree_.NearestSearchByLevel(exit_ray);

	// Condition
	if (itsc.is_hit_ == false)
	{
		temp_count1++;
		//return background;
		return V3(255,0,0);
	}
		

	if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
		/// Light
	{
		temp_count2++;
		return *itsc.pLe_;
	}

	if (depth > BLINN_PHONG_MAX_DEPTH)
		/// Out of maximum depth
	{
		temp_count3++;
		return light_[0]->Le_;
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
	float temp2 = pow(MAX2(0, Dot(itsc.normal_, H)), itsc.pMtl_->Ni_);
	color = itsc.pMtl_->Ni_*temp1*temp2;
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