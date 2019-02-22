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
	float Ni_;
	float Ns_;

	Material& operator=(Material& dat);
	void Clear();
};

