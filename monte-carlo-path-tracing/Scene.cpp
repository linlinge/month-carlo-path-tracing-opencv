#include "Scene.h"

void Scene::AddCamera(Camera camera)
{
	camera_ = camera;
}

void Scene::AddSphereLight(SphereLight dat)
{	
	objs_.push_back(&dat);
}

void Scene::AddQuadLight(QuadLight dat)
{	
	objs_.push_back(&dat);
}

void Scene::BuildKdTree()
{
	 tree_.Build(objs_, 0);
}

Mat Scene::Rendering()
{
	// Define
	Mat result(camera_.image_pixel_width_,camera_.image_pixel_height_,CV_8UC3,cv::Scalar(0,0,0));

	ofstream f("../matlab/ray.txt");

	// generate rays in each pixel	
	for (int i = 0; i < camera_.image_pixel_width_; i++)
	{
		for (int j = 0; j < camera_.image_pixel_height_; j++)
		{
			// Define
			V3 color;
			// Generate camera/primary ray					
			V3 pos = camera_.GetPosition(i, j);
			Ray ray;
			ray.origin_ = camera_.position_;
			ray.direction_ = (pos - camera_.position_).GetNorm();			
			f << ray.origin_ <<" "<<pos<< endl;			
			// Ray Tracing
			color=RayTracing(ray)*256;			
			result.at<Vec3b>(i, j) = Vec3b(color.b,color.g,color.r);
		}
	}
	f.close();
	return result;
}

// trace one ray 
V3 Scene::RayTracing(Ray& ray)
{
	V3 color;
	color = Lambertian(ray, 0);// +BlinnPhong(ray, 0);
	
	return color;
}

// difuss model
V3 Scene::Lambertian(Ray& exit_light, int depth)
{
	V3 color;
	Ray incident;
	// Get Nearest Patch
	Intersection itsc = tree_.NearestSearchByLevel(exit_light);

	if (itsc.is_hit_ == false)
		return V3(255, 0,0);


	// abnormal situation1: 
	if (itsc.type_ == SPHERE_SOURCE || itsc.type_ == QUAD_SOURCE)
	{
		return *itsc.pLe_;
	}



	// abnormal situation2: out of depth
	if (depth > 5)
		/// if out of depth
	{	
		srand((unsigned)time(NULL));
		float temp_random = rand() / double(RAND_MAX);
		if (temp_random < 0.1)
			return V3(0, 255, 0);
		else
			return V3(50, 50, 40);
	}

	// generate 4 incident ray
	for (int i = 0; i < LAMBERTIAN_SAMPLE_NUMBER; i++)
	{
		// Get Incident ray

		incident.origin_ = itsc.intersection_;
		incident.direction_ = GetRandom();
			
		// Get Intersection Material
		Material* mtl_temp = itsc.pMtl_;

		// caculate color
		color = color + mtl_temp->Kd_*Lambertian(incident, depth + 1)*Dot(exit_light.direction_,incident.direction_);
	}



	//float temp_arc = PI - GetArc(exit_light.direction_, itsc.normal_);
	//incident.origin_ = exit_light.origin_ + 2 * itsc.distance_*exit_l   ight.direction_ + 2 * itsc.distance_*cos(temp_arc)*itsc.normal_;
	//incident.direction_ = (itsc.intersection_ - incident.origin_).GetNorm();
	//// Get Intersection Material
	//Material* mtl_temp = itsc.pMtl_;

	//// caculate color
	//color = color + mtl_temp->Kd_*Lambertian(incident, depth + 1)*Dot(exit_light.direction_, incident.direction_);

	color = color / LAMBERTIAN_SAMPLE_NUMBER;	
	return color;
}

// specular model
V3 Scene::BlinnPhong(Ray& exit_light,int depth)
{
	//// get intersection
	//Intersection itsc = tree_.NearestSearchByLevel(exit_light);

	//// return color
	V3 color;

	//// specular light: use blinn-phong model
	//V3 light_direction = (sphere_light_[0].center_ - itsc.point_).GetNorm();
	//V3 view_direction = (camera_.position_ - itsc.point_).GetNorm();
	//V3 halfway_direction = (light_direction + view_direction).GetNorm();

	//// caculate color
	//Material& mtl_temp = mtls_[itsc.mtl_id_];
	//color = mtl_temp.Ks_*light_direction*pow(MAX2(0,Dot(itsc.normal_,halfway_direction)), mtl_temp.Ns_);
	
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

	//tree_.Build(objs_, 0);
}



Intersection Scene::GetIntersect(Ray& ray)
{
	return Intersection();
}