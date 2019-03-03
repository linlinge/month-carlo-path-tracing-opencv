#pragma once
#include <vector>
#include "V3.hpp"
#include "AABB.hpp"
#include "Patch.hpp"
#include "Ray.hpp"
#include <iostream>
#include <stack>
#define MAX_DEPTH 10
using namespace std;

class KdNode
{
public: 
	vector<Patch> leaf_val_;
	float internal_val_;
	int id_;
	int depth_;
	AABB box_;
	int axis_; // x:0,y:1,z:2	
	KdNode *left_,*right_;

	bool operator!=(KdNode& dat)
	{
		return id_ != dat.id_;
	}

	bool operator==(KdNode& dat)
	{
		return id_ == dat.id_;
	}

	KdNode()
	{
		id_ = INT_MAX;
		left_ = right_= NULL;
	}
};


class KdTree
{
public:
	KdNode * root_;
	int  max_depth_;
	vector<vector<int>> id_record_;
	
	KdNode* Build(vector<Patch>& f, int depth);
	Patch NearestSearch(Ray& ray);
	KdNode* NearestSearchRecursive(KdNode* node);

	void Print();
	void GetPrint(KdNode* head);
};


extern KdTree tree;