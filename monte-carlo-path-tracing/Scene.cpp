#include "Scene.h"
int count_temp = 0;
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
			// ray tracing
			color = RayTracing(i,j);
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

			/*if (i == 295 || j == 450)
			{
				result.at<Vec3b>(i, j)[0] = 255;
				result.at<Vec3b>(i, j)[1] = 0;
				result.at<Vec3b>(i, j)[2] = 0;
			}
			if (i == 78)
			{
				result.at<Vec3b>(i, j)[0] = 0;
				result.at<Vec3b>(i, j)[1] = 255;
				result.at<Vec3b>(i, j)[2] = 0;
			}*/
		}		
	}
	/*ofstream file("../output/distance.txt");
	for (int i = 0; i < IMAGE_WIDTH; i++)
	{
		for (int j = 0; j < IMAGE_HEIGHT; j++)
		{
			file << buf_temp[i*IMAGE_WIDTH + j] << " ";
		}
		file << endl;
	}
	file.close();*/
	// output result 
	cout << max_val << endl;
	cout << count_temp << endl;
	return result;
}

// trace one ray 
V3 Scene::RayTracing(int a,int b)
{
	// Define
	V3 color,pos;
	Ray ray;
	Intersection itsc;
	float scale = 0;
	V3 Lambertian_temp;
	V3 BlinnPhong_temp;

	// position
	pos = camera_.GetPosition(a, b);
	ray.Init(camera_.position_, (pos - camera_.position_).GetNorm());
	itsc = tree_.NearestSearchByLevel(ray);

	if (itsc.is_hit_ == true)
	{
		// shadow
		scale = ShadowTest(itsc, a, b);
		//scale = 1.0f;

		// Lambertain
		Lambertian_temp = Lambertian(itsc, a, b);		
	}
	
	// Blinn Phong
	for (int i = 0; i < BLINN_PHONG_SAMPLE_NUMBER; i++)
	{
		float x = a + GetRandomFloat() - 0.5f;
		float y = b + GetRandomFloat() - 0.5f;
		pos= camera_.GetPosition(x, y);
		ray.Init(camera_.position_, (pos - camera_.position_).GetNorm());
		itsc = tree_.NearestSearchByLevel(ray);
		if (itsc.is_hit_ == true)
		{
			BlinnPhong_temp = BlinnPhong_temp + BlinnPhong(ray, 0);
		}
	}
	BlinnPhong_temp = BlinnPhong_temp / (float)BLINN_PHONG_SAMPLE_NUMBER;
	// BlinnPhong_temp = V3(0, 0, 0);

	// merge
	color = scale * (0.5*BlinnPhong_temp +Lambertian_temp);
	return color;
}

// Shadow Test
float Scene::ShadowTest(Intersection& itsc,int a,int b)
{
	SphereLight* pSphereLight=NULL;
	QuadLight* pQuadLight = NULL;
	float scale = 0.0f;
	//if (a == 295 || b == 450)
	//	a = 295;
	//if (a == 79)
	//{
	//	a = 79;
	//}
	for (int i=0;i< light_.size();i++)
	{
		if (light_[i]->type_ == SPHERE_SOURCE)
		{
			//Object* light_temp = ;
			pSphereLight = static_cast<SphereLight*>(light_[i]);
			// Get random points from light
			Ray shadow_ray(itsc.intersection_, (pSphereLight->center_ - itsc.intersection_).GetNorm());
			Intersection shadow_itsc = tree_.NearestSearchByLevel(shadow_ray);
			if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == PATCH && shadow_itsc.distance_ > 0.0001f)
			// if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == PATCH && shadow_itsc.distance_ > 0.0001f)
			{
				scale = 0.4f;
			}
			else if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == SPHERE_SOURCE)
			{
				scale = 1.0f;
			}
			else
			{				
				/*cout << "Error" << endl;
				return 0;*/
				scale = 1.0f;
			}				
		}
		else if(light_[i]->type_==QUAD_SOURCE)
		{
			pQuadLight = static_cast<QuadLight*>(light_[i]);
			Ray shadow_ray(itsc.intersection_, (pQuadLight->center_ - itsc.intersection_).GetNorm());
			Intersection shadow_itsc = tree_.NearestSearchByLevel(shadow_ray);			
			if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == PATCH)// && shadow_itsc.distance_>0.0001f)
			{
				scale = 0.1f;
			}
			else if (shadow_itsc.is_hit_ == true && shadow_itsc.type_ == QUAD_SOURCE )
			{
				scale = 1.0f;
			}
			else
			{
				scale = 1.0f;
			}
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
		count_temp++;
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
		return background;		
	}
		
	if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
		/// Light
	{
		return *itsc.pLe_;
	}

	if (depth > BLINN_PHONG_MAX_DEPTH)
		/// Out of maximum depth
	{
		return background;
	}

	// Define 
	V3 color;
	// Get incident light	
	V3 L = (light_[0]->center_- itsc.intersection_).GetNorm();
	V3 V = (camera_.position_ - itsc.intersection_).GetNorm();
	V3 H = (L + V).GetNorm();

	// Caculate color
	Ray incident_ray = GetIncidentRay(exit_ray, itsc);
	V3 temp1 = BlinnPhong(incident_ray, depth + 1);
	//V3 temp1 = light_[0]->Le_;
	float temp2 = pow(MAX2(0, Dot(itsc.normal_, H)), itsc.pMtl_->Ns_);

	if (itsc.pMtl_->Ks_.x > 1.0f)
		itsc.pMtl_->Ks_ = V3(0.1f, 0.1f, 0.1f);

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