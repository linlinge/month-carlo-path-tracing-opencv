#pragma once
#include "V3.hpp"
#include "Ray.hpp"
class AABB
{
public:
	V3 top_left_, bottom_right_;
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
};

