#pragma once
#include "V3.hpp"
#include "Intersection.hpp"
#include "BasicGeometry.h"
#include "AABB.hpp"
#include "global.h"

class Object
{
public:
	ObjectType type_;
	V3 center_;
	AABB box_;
	virtual Intersection IsIntersect(Ray& ray) = 0;
	
};

class SphereLight :public Object
{
public:
	float radius_;	// light source radius
	V3 Le_;			// light source emission

	SphereLight(V3 center, float radius, V3 Le)
	{
		type_ = SPHERE_SOURCE;
		center_ = center;
		radius_ = radius;
		Le_ = Le;

		box_.top_left_ = center_ - radius_ / 2.0f;
		box_.bottom_right_ = center_ + radius_ / 2.0f;
	}

	SphereLight operator=(SphereLight& dat)
	{
		type_ = dat.type_;
		center_ = dat.center_;
		radius_ = dat.radius_;
		Le_ = dat.Le_;
		return *this;
	}

	virtual Intersection IsIntersect(Ray& ray)
	{
		Intersection itsc;
		itsc.type_ = SPHERE_SOURCE;
		
		V3 L1_vector = center_ - ray.origin_;
		float L1 = L1_vector.GetLength();
		float theta1 = acos(Dot(ray.direction_.GetNorm(), L1_vector.GetNorm()));
		float D = L1 * sin(theta1);
		if (D <= radius_)
		{
			itsc.is_hit_ = true;
			float theta2 = asin(D / radius_);
			itsc.distance_ = L1 / tan(theta1) - L1 / tan(theta2);
			itsc.pLe_ = &Le_;			
		}
		else
		{
			itsc.is_hit_ = false;
		}
		return itsc;
	}
};

class QuadLight :public Object
{
public:
	V3 normal_;			// light source normal
	float size_[2];		// size 
	vector<V3> vertex_;
	V3 Le_;				// light source emission

	QuadLight(V3 center, V3 normal, float size[2], V3 Le)
	{
		type_ = QUAD_SOURCE;
		center_ = center;
		normal_ = normal;
		size_[0] = size[0]; size_[1] = size[1];
		Le_ = Le;

		vertex_.push_back(V3(center_.x , center_.y+ sqrt(2)/2.0f, center_.z));
		vertex_.push_back(V3(center_.x, center_.y , center_.z+ sqrt(2) / 2.0f));
		vertex_.push_back(V3(center_.x , center_.y- sqrt(2) / 2.0f, center_.z));
		vertex_.push_back(V3(center_.x, center_.y , center_.z- sqrt(2) / 2.0f));

		for(auto& temp: vertex_)
			box_.Expand(temp);
	}

	virtual Intersection IsIntersect(Ray& ray)
	{
		Intersection itsc;
		itsc.type_ = QUAD_SOURCE;

		// step1: Solve for the intersection between ray and plane
		Plane plane1(vertex_[0], vertex_[1], vertex_[2]);
		Line line1(ray.origin_, ray.direction_);
		itsc.intersection_ = plane1.IsIntersect(line1);
		float is_same_direction = Dot(itsc.intersection_ - ray.origin_, (ray.direction_));
		if (is_same_direction < 0)
		{
			itsc.is_hit_ = false;
		}

		// Accumulate arc
		float accumulator = 0.0f;
		for (int i = 0; i < vertex_.size() - 1; i++)
		{
			Angle angle(itsc.intersection_, vertex_[i], vertex_[i+1]);
			accumulator += angle.arc_;
		}
		Angle angle(itsc.intersection_, vertex_[0], vertex_[3]);
		accumulator += angle.arc_;

		if (abs(accumulator - 2 * PI) < 0.01)
		{
			itsc.is_hit_ = true;
			itsc.distance_ = Distance(ray.origin_, itsc.intersection_);
			itsc.pLe_ = &Le_;
		}
		else
		{
			itsc.is_hit_ = false;
		}
		return itsc;		
	}

	QuadLight operator=(QuadLight dat)
	{
		center_ = dat.center_;
		normal_ = dat.normal_;
		size_[0]=dat.size_[0];
		size_[1] = dat.size_[1];
		Le_=dat.Le_;
	}
};


class Patch :public Object
{
public:
	vector<int> v_id_;
	vector<int> vt_id_;
	vector<int> vn_id_;
	V3 normal_;
	int obj_name_id_;
	Material* pMtl_;

	Patch operator=(Patch dat)
	{
		v_id_ = dat.v_id_;
		vt_id_ = dat.vt_id_;
		vn_id_ = dat.vn_id_;

		normal_ = dat.normal_;
		center_ = dat.center_;
		obj_name_id_ = dat.obj_name_id_;
		pMtl_ = dat.pMtl_;
		box_ = dat.box_;		
		return *this;
	}

	virtual Intersection IsIntersect(Ray& ray)
	{
		Intersection itsc;
		itsc.type_ = PATCH;

		// step1: Solve for the intersection between ray and plane
		Plane plane1(v_[v_id_[0]], v_[v_id_[1]], v_[v_id_[2]]);
		Line line1(ray.origin_, ray.direction_);
		itsc.intersection_= plane1.IsIntersect(line1);
		float is_same_direction = Dot(itsc.intersection_ - ray.origin_, (ray.direction_));
		if (is_same_direction < 0)
		{
			itsc.is_hit_ = false;
		}

		// Accumulate arc
		float accumulator = 0.0f;
		for (int i = 0; i < v_id_.size() - 1; i++)
		{
			Angle angle(itsc.intersection_, v_[v_id_[i]], v_[v_id_[i + 1]]);
			accumulator += angle.arc_;
		}
		Angle angle(itsc.intersection_, v_[v_id_[0]], v_[v_id_[v_id_.size() - 1]]);
		accumulator += angle.arc_;

		if (abs(accumulator - 2 * PI) < 0.01)
		{
			itsc.is_hit_ = true;
			itsc.distance_ = Distance(ray.origin_, itsc.intersection_);			
			itsc.pMtl_ = pMtl_;
			itsc.normal_ = normal_;
		}
		else
		{
			itsc.is_hit_ = false;

		}
		return itsc;
	}
};