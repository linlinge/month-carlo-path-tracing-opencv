#pragma once
//#include "Objects.h"
#include "Material.h"
#include "Global.h"
class Object;

class Intersection
{
public:
	bool is_hit_;
	ObjectType type_;
	float distance_;
	V3 intersection_;
	V3 normal_;
	Material* pMtl_;
	V3* pLe_;


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
		intersection_ = dat.intersection_;
		pMtl_= dat.pMtl_;
		pLe_ = dat.pLe_;
		return *this;
	}
};