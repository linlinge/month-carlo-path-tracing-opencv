#pragma once
#include <string>
#include "V3.hpp"
using namespace std;
class Material
{
public:
	string name_;
	int illum_;
	V3 Kd_, Ka_, Tf_, Ks_;
	float Ni_;  // light intensity for material surface
	float Ns_;	// specular factor

	Material& operator=(Material& dat)
	{
		name_ = dat.name_;
		illum_ = dat.illum_;
		Kd_ = dat.Kd_;
		Ka_ = dat.Ka_;
		Tf_ = dat.Tf_;
		Ks_ = dat.Ks_;
		Ni_ = dat.Ni_;
		Ns_ = dat.Ns_;
		return *this;
	}
	void Clear()
	{
		name_.clear();
		illum_ = INT_MAX;
		Kd_.Clear();
		Ka_.Clear();
		Tf_.Clear();
		Ks_.Clear();
		Ni_ = 0;
		Ns_ = 0;
	}
};

