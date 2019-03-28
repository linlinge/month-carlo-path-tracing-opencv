#pragma once
#include "V3.hpp"
class Ray
{
public:
	V3 origin_, direction_;	

	Ray& operator=(Ray& dat)
	{
		origin_ = dat.origin_;
		direction_ = dat.direction_;
		return *this;
	}
	Ray(V3 origin, V3 direction)
	{
		origin_ = origin;
		direction_ = direction;
	}
	Ray() {};
};