#include "BasicGeometry.h"

Line::Line(V3 Point, V3 Direction)
{
	a_[0] = Direction.x;
	a_[1] = Direction.y;
	a_[2] = Direction.z;
	b_[0] = Point.x;
	b_[1] = Point.y;
	b_[2] = Point.z;
}

V3 Line::IsIntersect(Plane& dat)
{
	float t = -(dat.A_*b_[0] + dat.B_*b_[1] + dat.C_*b_[2] + dat.D_) / (dat.A_*a_[0] + dat.B_*a_[1] + dat.C_*a_[2]);
	return V3(a_[0] * t + b_[0], a_[1] * t + b_[1], a_[2] * t + b_[2]);
}


Plane::Plane(V3 P1, V3 P2, V3 P3)
{
	A_ = (P2.y - P1.y)*(P3.z - P1.z) - (P3.y - P1.y)*(P2.z - P1.z);
	B_ = (P2.z - P1.z)*(P3.x - P1.x) - (P3.z - P1.z)*(P2.x - P1.x);
	C_ = (P2.x - P1.x)*(P3.y - P1.y) - (P3.x - P1.x)*(P2.y - P1.y);
	D_ = -A_ * P1.x - B_ * P1.y - C_ * P1.z;
}

V3 Plane::IsIntersect(Line& dat)
{
	float t = -(A_*dat.b_[0] + B_ * dat.b_[1] + C_ * dat.b_[2] + D_) / (A_*dat.a_[0] + B_ * dat.a_[1] + C_ * dat.a_[2]);
	return V3(dat.a_[0] * t + dat.b_[0], dat.a_[1] * t + dat.b_[1], dat.a_[2] * t + dat.b_[2]);
}


Angle::Angle(V3& mid, V3& left, V3& right)
{
	V3 line1 = left - mid;
	V3 line2 = right - mid;
	arc_=GetArc(line1,line2);
	angle_ = arc_ * 180.0 / PI;
}