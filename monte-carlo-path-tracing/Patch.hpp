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



	bool IsIntersect(Ray& ray)
	{
		// step1: Solve for the intersection between ray and plane
		Plane plane1(v_[v_id_[0]], v_[v_id_[1]], v_[v_id_[2]]);
		Line line1(ray.origin_,ray.direction_);
		V3 intersect_point = plane1.IsIntersect(line1);
		float is_same_direction = (intersect_point - ray.origin_).Dot(ray.direction_);
		if ( is_same_direction< 0)
			return false;

		// Accumulate arc
		float accumulator = 0.0f;
		for (int i = 0; i < v_id_.size()-1; i++)
		{
			Angle angle(intersect_point, v_[v_id_[i]], v_[v_id_[i + 1]]);
			accumulator += angle.arc_;
		}
		Angle angle(intersect_point, v_[v_id_[0]],v_[v_id_[v_id_.size()-1]]);
		accumulator += angle.arc_;

		if (abs(accumulator - 2*PI) < 0.01)
			return true;
		else
			return false;
	}
};
