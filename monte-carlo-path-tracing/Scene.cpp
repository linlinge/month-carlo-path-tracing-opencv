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
			color = RayTracing(ray,i,j);
			//cout << i << " " << j << endl;
			buf_temp[i*IMAGE_WIDTH + j] = color.r;
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

			/*if (i == 160 || j == 200)
			{
				result.at<Vec3b>(i, j)[0] = 255;
				result.at<Vec3b>(i, j)[1] = 0;
				result.at<Vec3b>(i, j)[2] = 0;
			}*/
		}		
	}
	ofstream file("../output/distance.txt");
	for (int i = 0; i < IMAGE_WIDTH; i++)
	{
		for (int j = 0; j < IMAGE_HEIGHT; j++)
		{
			file << buf_temp[i*IMAGE_WIDTH + j] << " ";
		}
		file << endl;
	}
	file.close();
	// output result 
	cout << max_val << endl;
	return result;
}

// trace one ray 
V3 Scene::RayTracing(Ray& ray,int a,int b)
{
	Intersection itsc = tree_.NearestSearchByLevel(ray);
	V3 color;
	if (a == 160 && b == 220)
		a = 160;

	if (a == 160 && b == 200)
		a = 160;

	if (itsc.is_hit_ == true)
	{
		float scale = ShadowTest(itsc, a, b);
		color = scale * (Lambertian(itsc,a,b));// +BlinnPhong(ray, 0));
	}
	else
		color = V3(0, 0, 0);
   	//V3 color = scale * V3(1,1,1);
	return color;
}

// Shadow Test
float Scene::ShadowTest(Intersection& itsc,int a,int b)
{
	SphereLight* pSphereLight=NULL;
	QuadLight* pQuadLight = NULL;
	float scale = 0.0f;
	if (a == 410 && b == 255)
 		a = 410;

	for (int i=0;i< light_.size();i++)
	{
		if (light_[i]->type_ == SPHERE_SOURCE)
		{
			//Object* light_temp = ;
			pSphereLight = static_cast<SphereLight*>(light_[i]);
			// Get random points from light
			Ray shadow_ray(itsc.intersection_, (pSphereLight->center_ - itsc.intersection_).GetNorm());
			Intersection shadow_itsc = tree_.NearestSearchByLevel(shadow_ray);
			if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == PATCH )
			{
				scale = 0.7f;
			}
			else if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == SPHERE_SOURCE)
			{
				scale = 1.0f;
			}
			else
			{
				scale = 0;
			}
				
		}
		else if(light_[i]->type_==QUAD_SOURCE)
		{
			pQuadLight = static_cast<QuadLight*>(light_[i]);
			Ray shadow_ray(itsc.intersection_, (pQuadLight->center_ - itsc.intersection_).GetNorm());
			Intersection shadow_itsc = tree_.NearestSearchByLevel(shadow_ray);			
			if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == PATCH )
			{
				scale = 0.9f;
			}
			else if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == QUAD_SOURCE )
			{
				scale = 1.0f;
			}
			else
				scale = 0;
		}
	}
	return scale;
}

// Difuss model
V3 Scene::Lambertian(Intersection& itsc,int a,int b)
{
	V3 color;	
	if (itsc.is_hit_ == false)
		/// not hit 
	{
		return background;
	}	


	if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
		/// abnormal situation1: hit light source
	{
		return *itsc.pLe_;
	}
	if (itsc.pMtl_ == NULL)
		/// no material
	{
		cout << "Error: no coresponding material file" << endl;
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
		//return background;
		return V3(255,0,0);
	}
		
	if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
		/// Light
	{
		return *itsc.pLe_;
	}

	if (depth > BLINN_PHONG_MAX_DEPTH)
		/// Out of maximum depth
	{
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