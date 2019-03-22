#pragma once
#include "AABB.hpp"
#include "KdTree.h"
#include "global.h"
#include "Material.hpp"
using namespace std;

class ObjFile {
public:
	vector<string> objs_name_;
	vector<Patch> f_;

	void LoadMaterial(string filename);
	void LoadObjs(string filename);
	void Clear();
	void GetProperties();
	int GetMtlId(string str);
};







