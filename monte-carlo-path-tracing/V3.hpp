#pragma once
#include <string>
#include <vector>
using namespace std;

class V3
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
	V3() :r(0.0f), g(0.0f), b(0.0f) {};
	V3(float r1, float g1, float b1)
	{
		r = r1;
		g = g1;
		b = b1;
	};
	V3 & operator=(const V3 & obj)
	{
		r = obj.r;
		g = obj.g;
		b = obj.b;
		return (*this);
	}

	

	float DistanceXY(V3 & dat)
	{
		return sqrt(pow(x - dat.x, 2.0f) + pow(y - dat.y, 2.0f));
	}
	float Distance(V3 & dat)
	{
		return sqrt(pow(x - dat.x, 2.0f) + pow(y - dat.y, 2.0f) + pow(z - dat.z, 2.0f));
	}

	V3& operator *(float scale)
	{
		x = x * scale;
		y = y * scale;
		z = z * scale;
		return *this;
	}
	V3& operator /(float scale)
	{
		x = x / scale;
		y = y / scale;
		z = z / scale;
		return *this;
	}
	V3& operator +(V3 & dat)
	{
		x = x + dat.x;
		y = y + dat.y;
		z = z + dat.z;
		return *this;
	}

	V3  operator -(V3 & dat)
	{
		V3 rst;
		rst.x = x - dat.x;
		rst.y = y - dat.y;
		rst.z = z - dat.z;
		return rst;
	}

	float GetArc(V3& dat)
	{
		float arc = x * dat.x + y * dat.y + z * dat.z;
		return arc / sqrt(x*x + y * y + z * z) / sqrt(dat.x*dat.x + dat.y*dat.y + dat.z*dat.z);
	}

	V3 Dot( V3& dat)
	{
		V3 rst;
		rst.x = y * dat.z - dat.y * z;
		rst.y = dat.x * z - x * dat.z;
		rst.z = x * dat.y - dat.x * y;
		return rst;
	}

	V3 Cross(V3& dat)
	{
		V3 rst;
		rst.x = y * dat.z - dat.y*z;
		rst.y = dat.x*z - x * dat.z;
		rst.z = x * dat.y - dat.x*y;
		return rst;
	}

	V3 GetNorm()
	{
		V3 rst;
		float a = sqrt(x*x + y * y + z * z);
		rst.y = y / a;
		rst.z = z / a;
		rst.x = x / a;
		return rst;
	}
	void Normalize()
	{
		float a = sqrt(x*x + y * y + z * z);
		y = y / a;
		z = z / a;
		x = x / a;
	}

	void clear()
	{
		x = y = z = INT_MAX;
	}
};



