#pragma once
#include <string>
#include <vector>
using namespace std;
class V2
{
public:
	float x, y;

	V2() { x = y = 0; }
	V2(float x1, float y1)
	{
		x = x1;
		y = y1;

	}
	V2& operator=(const V2 & obj)
	{
		x = obj.x;
		y = obj.y;

		return (*this);
	}
	bool operator==(V2& dat)
	{
		return ((x == dat.x) && (y == dat.y) );
	}
	bool operator!=(V2& dat)
	{
		if ((x == dat.x) && (y == dat.y) )
			return false;
		else
			return true;
	}


	
	friend float Distance(V2& dat1, V2& dat2)
	{
		return sqrt(pow(dat1.x - dat2.x, 2.0f) + pow(dat1.y - dat2.y, 2.0f));
	}

	V2 operator *(float scale)
	{
		V2 temp;
		temp.x = x * scale;
		temp.y = y * scale;
		return temp;
	}

	friend V2 operator*(float scale, V2 dat)
	{
		dat.x = scale * dat.x;
		dat.y = scale * dat.y;
		return dat;
	}

	V2 operator*(V2 dat)
	{
		dat.x = dat.x*x;
		dat.y = dat.y*y;
		return dat;
	}


	V2& operator /(float scale)
	{
		x = x / scale;
		y = y / scale;
		return *this;
	}
	friend V2 operator +(V2 dat1, V2 dat2)
	{
		V2 rst;
		rst.x = dat1.x + dat2.x;
		rst.y = dat1.y + dat2.y;
		return rst;
	}

	V2 operator+ (float dat)
	{
		V2 temp;
		temp.x = x + dat;
		temp.y = y + dat;
		return temp;
	}

	friend V2 operator+(float dat1, V2 dat2)
	{
		V2 temp;
		temp.x = dat1 + dat2.x;
		temp.y = dat1 + dat2.y;
		return temp;
	}


	V2  operator -(V2  dat)
	{
		V2 rst;
		rst.x = x - dat.x;
		rst.y = y - dat.y;
		return rst;
	}
	V2 operator-(float dat)
	{
		V2 rst;
		rst.x -= dat;
		rst.y -= dat;
		return rst;
	}


	friend float GetArc(V2 dat1, V2 dat2)
	{
		float cos_theta = Dot(dat1.GetNorm(), dat2.GetNorm());
		float theta = acos(cos_theta);
		return theta;
	}

	friend float Dot(V2 dat1, V2 dat2)
	{
		return (dat1.x*dat2.x + dat1.y*dat2.y );
	}

	V2 GetNorm()
	{
		V2 rst;
		float a = sqrt(x*x + y * y );
		rst.y = y / a;
		rst.x = x / a;
		return rst;
	}

	float GetLength()
	{
		return sqrt(x*x + y * y );
	}


	void Normalize()
	{
		float a = sqrt(x*x + y * y );
		y = y / a;
		x = x / a;
	}

	void Clear()
	{
		x = y= INT_MAX;
	}

	friend ostream& operator <<(ostream& os, const V2 dat)
	{
		os << dat.x << " " << dat.y;
		return os;
	}
};