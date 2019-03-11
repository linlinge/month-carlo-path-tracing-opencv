#pragma once
#include "V3.hpp"
#include "Ray.hpp"
#include "Objects.h"

class Intersection
{
public:
	bool is_hit_;	
	float distance_;
	Object* object_;

	Intersection operator =(Intersection& dat)
	{
		is_hit_ = dat.is_hit_;
		distance_ = dat.distance_;
		object_ = dat.object_;
		return *this;
	}
};
