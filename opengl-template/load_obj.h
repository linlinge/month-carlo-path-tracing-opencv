#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "main.h"
using namespace std;
void remove_adjacent_duplicate(string& dat1, string dat2);
vector<string> split(string dat, string separator);

class Material
{
public:
	string name_;
	int illum_;
	Vec3f Kd_, Ka_, Tf_, Ks_;
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
class Materials
{
public:
	vector<Material> materials_;
	int get_id(string name)
	{
		for (int i=0;i<materials_.size();i++)
		{
			if (materials_[i].name_== name)
			{
				return i;
			}
		}
	}
	bool Load(string filename);
};

struct SmoothGroup {
	int status_;
	int position_;
};

struct face {
	vector<int> v_id_;
	vector<int> vt_id_;
	vector<int> vn_id_;
	Vec3f normal_;
};

class Group {
public:
	string group_name_;
	string usemtl_;
	vector<face> f_;	
	vector<SmoothGroup> s_;
	int mtl_id_;

	void clear() {
		group_name_.clear();
		usemtl_.clear();
		f_.clear();
		s_.clear();
	}
};

class Objs {
public:
	vector<Vec3f> v_;
	vector<Vec3f> vt_;
	vector<Vec3f> vn_;
	vector<Group> groups_;
	Materials mtls_;
	void Load(string filename);
};





extern Objs objs1;
extern Materials mtls1;
