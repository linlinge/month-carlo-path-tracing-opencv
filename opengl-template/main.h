#pragma once
#include <string>
#include <vector>
using namespace std;
#define TOSCREEN(DAT,MIN_VAL,MAX_VAL) (1.6*((DAT-MIN_VAL)/(MAX_VAL-MIN_VAL)-0.5))


class Vec3f
{
public:
	union {
		struct {
			float x, y, z;
		};

		struct {
			float r, g, b;
		};
	};
	Vec3f() :r(0.0f), g(0.0f), b(0.0f) {};
	Vec3f(float r1, float g1, float b1)
	{
		r = r1;
		g = g1;
		b = b1;
	};
	Vec3f & operator=(const Vec3f & obj)
	{
		r = obj.r;
		g = obj.g;
		b = obj.b;
		return (*this);
	}

	

	float DistanceXY(Vec3f & dat)
	{
		return sqrt(pow(x - dat.x, 2.0f) + pow(y - dat.y, 2.0f));
	}
	float Distance(Vec3f & dat)
	{
		return sqrt(pow(x - dat.x, 2.0f) + pow(y - dat.y, 2.0f) + pow(z - dat.z, 2.0f));
	}

	Vec3f& operator *(float scale)
	{
		x = x * scale;
		y = y * scale;
		z = z * scale;
		return *this;
	}
	Vec3f& operator /(float scale)
	{
		x = x / scale;
		y = y / scale;
		z = z / scale;
		return *this;
	}
	Vec3f& operator +(Vec3f & dat)
	{
		x = x + dat.x;
		y = y + dat.y;
		z = z + dat.z;
		return *this;
	}

	Vec3f  operator -(Vec3f & dat)
	{
		Vec3f rst;
		rst.x = x - dat.x;
		rst.y = y - dat.y;
		rst.z = z - dat.z;
		return rst;
	}

	float GetArc(Vec3f& dat)
	{
		float arc = x * dat.x + y * dat.y + z * dat.z;
		return arc / sqrt(x*x + y * y + z * z) / sqrt(dat.x*dat.x + dat.y*dat.y + dat.z*dat.z);
	}

	Vec3f Dot( Vec3f& dat)
	{
		Vec3f rst;
		rst.x = y * dat.z - dat.y * z;
		rst.y = dat.x * z - x * dat.z;
		rst.z = x * dat.y - dat.x * y;
		return rst;
	}

	Vec3f Cross(Vec3f& dat)
	{
		Vec3f rst;
		rst.x = y * dat.z - dat.y*z;
		rst.y = dat.x*z - x * dat.z;
		rst.z = x * dat.y - dat.x*y;
		return rst;
	}

	Vec3f Normalize()
	{
		Vec3f rst;
		float a = sqrt(x*x + y * y + z * z);
		rst.y = y / a;
		rst.z = z / a;
		rst.x = x / a;
		return rst;
	}

	void clear()
	{
		x = y = z = INT_MAX;
	}
};



