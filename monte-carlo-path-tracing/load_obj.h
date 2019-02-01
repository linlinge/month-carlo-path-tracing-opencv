#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "V3.hpp"
#include "AABB.h"
using namespace std;


void remove_adjacent_duplicate(string& dat1, string dat2);
vector<string> split(string dat, string separator);

class Material
{
public:
	string name_;
	int illum_;
	V3 Kd_, Ka_, Tf_, Ks_;
	float Ni_;
	float Ns_;

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
	}

	void clear()
	{
		name_.clear();
		illum_ = INT_MAX;
		Kd_.clear();
		Ka_.clear();
		Tf_.clear();
		Ks_.clear();
		Ni_ = 0;
		Ns_ = 0;
	}
};
extern vector<V3> g_v;
extern vector<V3> g_vt;
extern vector<V3> g_vn;
extern vector<Material> g_mtls;

struct SmoothGroup {
	int status_;
	int position_;
};

class face {
public:
	vector<int> v_id_;
	vector<int> vt_id_;
	vector<int> vn_id_;
	V3 normal_;
	AABB box_;	
};

class Obj {
public:	
	string obj_name_;
	string usemtl_;
	vector<face> f_;
	int mtl_id_;
	vector<SmoothGroup> s_;

	void clear()
	{
		obj_name_.clear();
		usemtl_.clear();
		f_.clear();
		mtl_id_ = -INT_MAX;
		s_.clear();
	}

	int get_mtl_id(string str)
	{
		for (int i=0;i<g_mtls.size();i++)
		{
			if (g_mtls[i].name_ == str)
			{
				return i;
			}
		}
		return -1;
	}
};




void LoadMaterial(string filename);
void LoadObj(string filename, vector<Obj>& dat);





