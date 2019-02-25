#include "kd_tree.h"
KdTree tree;

KdNode* KdTree::Build(vector<Patch>& f,int depth)
{
	//cout << depth << endl;
	/*if (depth == 7)
		depth = 7;*/
	// define
	KdNode *node = new KdNode;

	// find box for this node
	AABB max_box;
	max_box = f[0].box_;
	for (int i = 1; i < f.size(); i++)
	{
		max_box.Expand(f[i].center_);
	}
	node->box_ = max_box;

	// leaf node or not
	if (depth > MAX_DEPTH || f.size()==0)
	{
		node->is_leaf_ = true;
		node->leaf_val_ = f;
		return node;
	}

	// internal node
	node->is_leaf_ = false;
	
	// get the axis
	node->axis_ = node->box_.GetMaxAxis();

	// splite data
	/// get middle value
	node->internal_val_ = 0;
	for (int i = 0; i < f.size(); i++)
	{
		switch (node->axis_)
		{
		case 0:
			node->internal_val_ += f[i].center_.x;
			break;
		case 1:
			node->internal_val_ += f[i].center_.y;
			break;
		case 2:
			node->internal_val_ += f[i].center_.z;
			break;
		}
	}
	node->internal_val_ = node->internal_val_ / f.size();

	/// 
	vector<Patch> left, right;	
	switch (node->axis_)
	{
	case 0:
		for (int i = 0; i < f.size(); i++)
		{
			if (f[i].center_.x < node->internal_val_)
				left.push_back(f[i]);
			else
				right.push_back(f[i]);
		}
		break;
	case 1:
		for (int i = 0; i < f.size(); i++)
		{
			if (f[i].center_.y < node->internal_val_)
				left.push_back(f[i]);
			else
				right.push_back(f[i]);			
		}
		break;
	case 2:
		for (int i = 0; i < f.size(); i++)
		{
			if (f[i].center_.z < node->internal_val_)
				left.push_back(f[i]);
			else
				right.push_back(f[i]);
		}
		break;
	}	

	if (left.size() > 0)
		node->left_ = Build(left, depth + 1);
	else
		node->left_ = NULL;

	if (right.size() > 0)
		node->right_ = Build(right, depth + 1);
	else
		node->right_ = NULL;

	// root or not
	if (depth == 0)
	{
		root_ = node;
	}
	return node;
}

Patch KdTree::NearestSearch(Ray& ray)
{


}