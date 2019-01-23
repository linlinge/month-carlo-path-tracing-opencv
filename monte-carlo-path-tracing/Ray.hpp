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

	RayIntersection IntersectionWithBox()
	{

	}
};