#pragma once
#include "V3.hpp"
#include "Ray.hpp"
#include <algorithm>
class AABB
{
public:
	V3 top_left_, bottom_right_;

	AABB() {};
	AABB(V3 top_left, V3 bottom_right)
	{
		top_left_ = top_left;
		bottom_right_ = bottom_right;
	}

	void Expand(V3& p)
	{
		// x axis
		top_left_.x = top_left_.x < p.x ? top_left_.x : p.x;
		bottom_right_.x = bottom_right_.x > p.x ? bottom_right_.x : p.x;

		// y axis
		top_left_.y = top_left_.y < p.y ?  top_left_.y : p.y;
		bottom_right_.y = bottom_right_.y > p.y ? bottom_right_.y : p.y;

		// z axis
		top_left_.z = top_left_.z < p.z ? top_left_.z : p.z;
		bottom_right_.z = bottom_right_.z > p.z ? bottom_right_.z : p.z;
	}

	void Expand(AABB& dat)
	{
		top_left_.x = top_left_.x < dat.top_left_.x ? top_left_.x : dat.top_left_.x;
		top_left_.y = top_left_.y < dat.top_left_.y ? top_left_.y : dat.top_left_.y;
		top_left_.z = top_left_.z < dat.top_left_.z ? top_left_.z : dat.top_left_.z;

		bottom_right_.x = bottom_right_.x > dat.bottom_right_.x ? bottom_right_.x : dat.bottom_right_.x;
		bottom_right_.y = bottom_right_.y > dat.bottom_right_.y ? bottom_right_.y : dat.bottom_right_.y;
		bottom_right_.z = bottom_right_.z > dat.bottom_right_.z ? bottom_right_.z : dat.bottom_right_.z;
	}


	AABB&  operator=(const AABB& dat)
	{
		top_left_ = dat.top_left_;
		bottom_right_ = dat.bottom_right_;
		return *this;
	}
	

	int GetMaxAxis()
	{
		float delta_x = bottom_right_.x - top_left_.x;
		float delta_y = bottom_right_.y - top_left_.y;
		float delta_z = bottom_right_.z - top_left_.z;

		if (delta_x >= delta_y && delta_x >= delta_z) return 0;
		if (delta_y >= delta_x && delta_y >= delta_z) return 1;
		if (delta_z >= delta_x && delta_z >= delta_y) return 2;
	}

	bool IsIntersect(Ray& ray)
	{
		float tmin = 0.0f;
		float tmax = FLT_MAX;

		//The plane perpendicular to x-axie
		if (abs(ray.direction_.x) < 0.000001f) //If the ray parallel to the plane
		{
			//If the ray is not within AABB box, then not intersecting
			if (ray.origin_.x < top_left_.x || ray.origin_.x > bottom_right_.x)
				return false;
		}
		else
		{
			//Compute the distance of ray to the near plane and far plane
			float ood = 1.0f / ray.direction_.x;
			float t1 = (top_left_.x - ray.origin_.x) * ood;
			float t2 = (bottom_right_.x - ray.origin_.x) * ood;

			//Make t1 be intersecting with the near plane, t2 with the far plane
			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			//Compute the intersection of slab intersection intervals
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;

			//Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax) return false;
		}// end for perpendicular to x-axie

		//The plane perpendicular to y-axie
		if (abs(ray.direction_.y) < 0.000001f) //If the ray parallel to the plane
		{
			//If the ray is not within AABB box, then not intersecting
			if (ray.origin_.y < top_left_.y || ray.origin_.y > bottom_right_.y)
				return false;
		}
		else
		{
			//Compute the distance of ray to the near plane and far plane
			float ood = 1.0f / ray.direction_.y;
			float t1 = (top_left_.y - ray.origin_.y) * ood;
			float t2 = (bottom_right_.y - ray.origin_.y) * ood;

			//Make t1 be intersecting with the near plane, t2 with the far plane
			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			//Compute the intersection of slab intersection intervals
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;

			//Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax) return false;
		}// end for perpendicular to y-axie

		//The plane perpendicular to z-axie
		if (abs(ray.direction_.z) < 0.000001f) //If the ray parallel to the plane
		{
			//If the ray is not within AABB box, then not intersecting
			if (ray.origin_.z < top_left_.z|| ray.origin_.z > bottom_right_.z)
				return false;
		}
		else
		{
			//Compute the distance of ray to the near plane and far plane
			float ood = 1.0f / ray.direction_.z;
			float t1 = (top_left_.z- ray.origin_.z) * ood;
			float t2 = (bottom_right_.z - ray.origin_.z) * ood;

			//Make t1 be intersecting with the near plane, t2 with the far plane
			if (t1 > t2)
			{
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}

			//Compute the intersection of slab intersection intervals
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;

			//Exit with no collision as soon as slab intersection becomes empty
			if (tmin > tmax) return false;
		}// end for perpendicular to z-axie

		/*vcHit->x = ray.origin_.x + tmin * ray.direction_.x;
		vcHit->y = ray.origin_.y + tmin * ray.direction_.y;
		vcHit->z = ray.origin_.z + tmin * ray.direction_.z;*/
		return true;	
	}
};

