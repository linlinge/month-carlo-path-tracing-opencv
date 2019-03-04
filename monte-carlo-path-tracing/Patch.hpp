#pragma once
#include <vector>
#include "V3.hpp"
#include "AABB.hpp"
#include "BasicGeometry.h"
using namespace std;


extern vector<V3> v_;
extern vector<V3> vt_;
extern vector<V3> vn_;

class Patch {
public:
	vector<int> v_id_;
	vector<int> vt_id_;
	vector<int> vn_id_;
	V3 normal_;
	V3 center_;
	int obj_name_id_;
	int mtl_id_;
	AABB box_;
	V3 intersect_point_;

	Patch& operator=(Patch& dat)
	{
		v_id_ = dat.v_id_;
		vt_id_ = dat.vt_id_;
		vn_id_ = dat.vn_id_;

		normal_ = dat.normal_;
		center_ = dat.center_;
		obj_name_id_ = dat.obj_name_id_;
		mtl_id_ = dat.mtl_id_;
		box_ = dat.box_;
		intersect_point_ = dat.intersect_point_;

		return *this;
	}

	float IsIntersect(Ray& ray)
	{
		// step1: Solve for the intersection between ray and plane
		Plane plane1(v_[v_id_[0]], v_[v_id_[1]], v_[v_id_[2]]);
		Line line1(ray.origin_,ray.direction_);
		intersect_point_ = plane1.IsIntersect(line1);
		float is_same_direction = (intersect_point_ - ray.origin_).Dot(ray.direction_);
		if ( is_same_direction< 0)
			return false;

		// Accumulate arc
		float accumulator = 0.0f;
		for (int i = 0; i < v_id_.size()-1; i++)
		{
			Angle angle(intersect_point_, v_[v_id_[i]], v_[v_id_[i + 1]]);
			accumulator += angle.arc_;
		}
		Angle angle(intersect_point_, v_[v_id_[0]],v_[v_id_[v_id_.size()-1]]);
		accumulator += angle.arc_;

		if (abs(accumulator - 2 * PI) < 0.01)
		{
			return ray.origin_.Distance(intersect_point_);
		}		
		else
		{
			return -1.0f;
		}
	}
};
