#pragma once
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "V3.hpp"
#include "AABB.h"
#include "kd_tree.h"
#include "Patch.h"
#include "Material.h"
using namespace std;
void remove_adjacent_duplicate(string& dat1, string dat2);
vector<string> split(string dat, string separator);



class Objs {
public:	
	
	vector<Material> mtls_;
	vector<string> objs_name_;
	vector<Patch> f_;

	void LoadMaterial(string filename);
	void LoadObjs(string filename);
	void Clear();
	void GetProperties();
	int GetMtlId(string str);
};







