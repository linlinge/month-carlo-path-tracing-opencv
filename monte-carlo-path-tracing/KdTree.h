#pragma once
#include <vector>
#include "V3.hpp"
#include "AABB.hpp"
#include "Ray.hpp"
#include <iostream>
#include <stack>
#include "Objects.hpp"

#define MAX_DEPTH 10
using namespace std;

class KdNode
{
public: 
	Object* leaf_val_;
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
	int leaf_num_;
	KdTree()
	{
		root_ = NULL;
		max_depth_ = 0;
		leaf_num_ = 0;
	}
	vector<vector<int>> id_record_;	
	KdNode* Build(vector<Object*>& objs, int depth);
	Intersection NearestSearchByLevel(Ray& ray); // level search	
	void Print();
	void GetPrint(KdNode* head);
	float  GetMean(vector<Object*>& dat, int axis);
};