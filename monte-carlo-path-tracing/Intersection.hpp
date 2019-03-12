#pragma once
//#include "Objects.h"
#include "Material.h"
class Object;

class Intersection
{
public:
	bool is_hit_;
	float distance_;
	V3 intersection_;
	Material* pMtl_;

	Intersection()
	{
		is_hit_ = false;
		distance_ = INT_MAX;
		pMtl_ = NULL;
	}

	Intersection operator =(Intersection& dat)
	{		
		is_hit_ = dat.is_hit_;
		distance_ = dat.distance_;
		V3 intersection_ = dat.intersection_;
		pMtl_= dat.pMtl_;
		return *this;
	}
};