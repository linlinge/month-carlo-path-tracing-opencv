#pragma once
#include <vector>
#include "V3.hpp"
#include "AABB.hpp"
#include "Patch.hpp"
#include "Ray.hpp"
#include <iostream>
#define MAX_DEPTH 10
using namespace std;

class KdNode
{
public: 
	vector<Patch> leaf_val_;
	float internal_val_;

	bool is_leaf_;
	AABB box_;
	int axis_; // x:0,y:1,z:2	
	KdNode *left_,*right_;
};


class KdTree
{
public:
	KdNode * root_;
	KdNode* Build(vector<Patch>& f, int depth);
	Patch NearestSearch(Ray& ray);
};


extern KdTree tree;