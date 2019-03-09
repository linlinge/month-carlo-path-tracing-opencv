#pragma once
#include "V3.hpp"
class Ray
{
public:
	V3 origin_, direction_;	
	V3 color_;


	Ray operator=(Ray& dat)
	{
		origin_ = dat.origin_;
		direction_ = dat.direction_;
	}
};