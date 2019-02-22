#pragma once
#include "V3.hpp"
#include "Ray.hpp"
#include <algorithm>
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

	bool IsIntersect(Ray& ray)
	{
		float ox = ray.origin_.x; float oy = ray.origin_.y; float oz = ray.origin_.z;
		float dx = ray.direction_.x; float dy = ray.direction_.y; float dz = ray.direction_.z;
		float tx_min, ty_min, tz_min;
		float tx_max, ty_max, tz_max;
		float x0 = top_left_.x;
		float y0 = top_left_.y;
		float z0 = top_left_.z;
		float x1 = bottom_right_.x;
		float y1 = bottom_right_.y;
		float z1 = bottom_right_.z;

		//x0,y0,z0为包围体的最小顶点  
		//x1,y1,z1为包围体的最大顶点  
		if (abs(dx) < 0.000001f)
		{
			//若射线方向矢量的x轴分量为0且原点不在盒体内  
			if (ox < x1 || ox > x0)
				return false;
		}
		else
		{
			if (dx >= 0)
			{
				tx_min = (x0 - ox) / dx;
				tx_max = (x1 - ox) / dx;
			}
			else
			{
				tx_min = (x1 - ox) / dx;
				tx_max = (x0 - ox) / dx;
			}

		}


		if (abs(dy) < 0.000001f)
		{
			//若射线方向矢量的x轴分量为0且原点不在盒体内  
			if (oy < y1 || oy > y0)
				return false;
		}
		else
		{
			if (dy >= 0)
			{
				ty_min = (y0 - oy) / dy;
				ty_max = (y1 - oy) / dy;
			}
			else
			{
				ty_min = (y1 - oy) / dy;
				ty_max = (y0 - oy) / dy;
			}

		}


		if (abs(dz) < 0.000001f)
		{
			//若射线方向矢量的x轴分量为0且原点不在盒体内  
			if (oz < z1 || oz > z0)
				return false;
		}
		else
		{
			if (dz >= 0)
			{
				tz_min = (z0 - oz) / dz;
				tz_max = (z1 - oz) / dz;
			}
			else
			{
				tz_min = (z1 - oz) / dz;
				tz_max = (z0 - oz) / dz;
			}

		}

		double t0, t1;

		//光线进入平面处（最靠近的平面）的最大t值   
		t0 = max(tz_min, max(tx_min, ty_min));

		//光线离开平面处（最远离的平面）的最小t值  
		t1 = min(tz_max, min(tx_max, ty_max));

		return t0 < t1;
	}
};

