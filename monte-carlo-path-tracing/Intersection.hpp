#pragma once
#include "V3.hpp"
#include "Ray.hpp"
enum IntersectionType { PATCH, SPHERE };
class Intersection
{
public:
	bool is_hit_;
	IntersectionType type_;
	int mtl_id_;
	float distance_;
	V3 point_;
	V3 normal_;
	Ray incident_light_;

	Intersection operator =(Intersection& dat)
	{
		is_hit_ = dat.is_hit_;
		type_ = dat.type_;
		mtl_id_ = dat.mtl_id_;
		distance_ = dat.distance_;
		point_ = dat.point_;
		normal_ = dat.normal_;
		incident_light_ = dat.incident_light_;
		return *this;
	}
};
