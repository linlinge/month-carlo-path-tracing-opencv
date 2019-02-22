#pragma once
#include <vector>
#include "V3.hpp"
#include "AABB.h"
using namespace std;
class Patch {
public:
	vector<int> v_id_;
	vector<int> vt_id_;
	vector<int> vn_id_;
	V3 normal_;
	V3 center_;
	int obj_name_id_;
	int mtl_id_;

	AABB box_;
};
