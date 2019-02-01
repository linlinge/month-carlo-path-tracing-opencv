#pragma once
#include "V3.hpp"
#include "Ray.hpp"
class AABB
{
public:
	V3 min_, max_;

	bool IsIntersect(Ray& ray);
};

