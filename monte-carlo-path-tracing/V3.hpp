#pragma once
#include <string>
#include <vector>
using namespace std;
#define PI 3.1415926535897
#define MAX2(a,b) (a>b? a:b)

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
	}
	V3& operator=(const V3 & obj)
	{
		r = obj.r;
		g = obj.g;
		b = obj.b;
		return (*this);
	}
	bool operator==(V3& dat)
	{
		return ((x == dat.x) && (y == dat.y) && (z == dat.z));
	}
	bool operator!=(V3& dat)
	{
		if ((x == dat.x) && (y == dat.y) && (z == dat.z))
			return false;
		else
			return true;		
	}
	

	float DistanceXY(V3 & dat)
	{
		return sqrt(pow(x - dat.x, 2.0f) + pow(y - dat.y, 2.0f));
	}
	float Distance(V3 & dat)
	{
		return sqrt(pow(x - dat.x, 2.0f) + pow(y - dat.y, 2.0f) + pow(z - dat.z, 2.0f));
	}

	V3 operator *(float scale)
	{
		V3 temp;
		temp.x = x * scale;
		temp.y = y * scale;
		temp.z = z * scale;
		return temp;
	}

	friend V3 operator*(float scale, V3 dat)
	{
		dat.x = scale * dat.x;
		dat.y = scale * dat.y;
		dat.z = scale * dat.z;
		return dat;
	}

	V3 operator*(V3 dat)
	{
		dat.x = dat.x*x;
		dat.y = dat.y*y;
		dat.z = dat.z*z;
		return dat;
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
		float cos_theta= arc / sqrt(x*x + y * y + z * z) / sqrt(dat.x*dat.x + dat.y*dat.y + dat.z*dat.z);
		float theta=acos(cos_theta);		
		return theta;
	}

	friend float Dot(V3 dat1,V3 dat2)
	{	
		return (dat1.x*dat2.x+dat1.y*dat2.y+dat1.z*dat2.z);
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

	void Clear()
	{
		x = y = z = INT_MAX;
	}

	friend ostream& operator <<(ostream& os, const V3 dat)
	{
		os << dat.x << " " << dat.y << " " << dat.z;
		return os;
	}
};
