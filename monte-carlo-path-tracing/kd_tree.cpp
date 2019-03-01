#include "kd_tree.h"
#include <fstream>
#include <iostream>
using namespace std;

KdTree tree;
int number = 0;

KdNode* KdTree::Build(vector<Patch>& f,int depth)
{
	//cout << depth << endl;
	/*if (depth == 7)
		depth = 7;*/

	// store depth
	max_depth_ = max_depth_ > depth ? max_depth_ : depth;

	// define
	KdNode *node = new KdNode;
	node->id_ = number++;
	node->depth_ = depth;

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
		node->leaf_val_ = f;
		return node;
	}

	
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
	KdNode node_temp;
	stack<KdNode> s;
	s.push(*root_);

	do
	{
		//left child 
		while (s.top().left_ != NULL) s.push(*s.top().left_);

		// left end, right child
		/// leaf node
		if (s.top().right_ == NULL)
		{
			s.pop(); 
		}
		else
		{
			s.push(*s.top().right_);
		}
		

	} while (s.top() != *root_);


	return Patch();
}

void KdTree::Print()
{
	id_record_.resize(max_depth_+1);
	GetPrint(root_);

	ofstream file("../output/kdtree-id-list.txt");
	for (int i = 0; i < id_record_.size(); i++)
	{
		for (int j = 0; j < id_record_[i].size(); j++)
		{
			file << id_record_[i][j] << " ";
		}
		file << endl;
	}
	file.close();	
}

int temp = 0;
void KdTree::GetPrint(KdNode* head)
{
	/*temp++;
	cout << temp << endl;
	if (temp == 17)
		temp = 17;*/

	if (head != NULL)
	{
		id_record_[head->depth_].push_back(head->id_);
	}
	else
		return;

	GetPrint(head->left_);
	GetPrint(head->right_);
	
}