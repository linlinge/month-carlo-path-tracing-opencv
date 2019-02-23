#pragma once
#include "V3.hpp"

class Plane;

class Line
{
public:
	// represent by parameter
	float a_[3], b_[3]; /// a_ represnet the slope parameter, b represent the intercept
	
	// convert point-direction equation to parameter equation
	Line(V3 Point, V3 Direction);
	V3 IsIntersect(Plane& dat);
};


class Plane
{
public:
	float A_, B_, C_, D_;

	Plane(V3 P1, V3 P2, V3 P3);
	V3 IsIntersect(Line& dat);
};


class Angle
{
public:
	float arc_, angle_;

	Angle(V3& mid,V3& left,V3& right);
};