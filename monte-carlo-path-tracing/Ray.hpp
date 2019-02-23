#pragma once
#include "V3.hpp"

class RayIntersection
{
public:
	float t;
	V3 color;
};

class Ray
{
public:
	V3 origin_, direction_;	

	Ray operator=(Ray& dat)
	{
		origin_ = dat.origin_;
		direction_ = dat.direction_;
	}
};