#include "KdTree.h"
#include <fstream>
#include <iostream>
using namespace std;
int number = 0;

KdNode* KdTree::Build(vector<Object*>& objs,int depth)
{
	/// Define
	vector<Object*> left, right;

	// store depth
	max_depth_ = max_depth_ > depth ? max_depth_ : depth;

	// define
	KdNode *node = new KdNode;
	node->id_ = number++;
	node->depth_ = depth;

	// find box for this node
	AABB max_box;
	max_box = objs[0]->box_;
	for (int i = 1; i < objs.size(); i++)
	{
		max_box.Expand(objs[i]->box_);
	}
	node->box_ = max_box;

	// leaf node or not
	//if (depth > MAX_DEPTH || objs.size()==1)
	if ( objs.size() == 1)
	{
		node->leaf_val_ = objs[0];
		leaf_num_++;
		return node;
	}

	// get the axis
	node->axis_ = node->box_.GetMaxAxis();

	// splite data
	/// get mean value
	node->internal_val_ = GetMean(objs,node->axis_);
	/// equal condition
	while (node->internal_val_ == INT_MAX)
	{
		node->axis_ = (node->axis_ + 1) % 3;
		node->internal_val_ = GetMean(objs, node->axis_);
	}
		
	switch (node->axis_)
	{
	case 0:
		for (int i = 0; i < objs.size(); i++)
		{
			if (objs[i]->center_.x < node->internal_val_)
				left.push_back(objs[i]);
			else
				right.push_back(objs[i]);
		}
		break;
	case 1:
		for (int i = 0; i < objs.size(); i++)
		{
			if (objs[i]->center_.y < node->internal_val_)
				left.push_back(objs[i]);
			else
				right.push_back(objs[i]);			
		}
		break;
	case 2:
		for (int i = 0; i < objs.size(); i++)
		{
			if (objs[i]->center_.z < node->internal_val_)
				left.push_back(objs[i]);
			else
				right.push_back(objs[i]);
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

Intersection KdTree::NearestSearchByLevel(Ray& ray)
{
	vector<KdNode> v;
	float min_dist = INT_MAX;
	int min_id = INT_MAX;
	v.push_back(*root_);
	int current_level_num = 0;
	int current_depth = 0;
	Intersection nearest_itsc;

	if (root_->box_.IsIntersect(ray) == true)
	{
		while (current_depth <= max_depth_)
		{
			// get node number of current level
			current_level_num = v.size();

			// deal with each level
			for (int i = 0; i < current_level_num; i++)
			{
				if (v[i].box_.IsIntersect(ray) == true)
					/// if has intersection
				{
					/// leaf node ?
					if (v[i].left_ == NULL && v[i].right_ == NULL)
						/// leaf node
					{
						Intersection itsc;
						switch (v[i].leaf_val_[0].type_)
						{
						case PATCH:
							itsc=((Patch*) &v[i].leaf_val_[0])->IsIntersect(ray);
							break;
						case SPHERE_SOURCE:
							itsc = ((SphereLight*)&v[i].leaf_val_[0])->IsIntersect(ray);
							break;
						case QUAD_SOURCE:
							itsc = ((QuadLight*)&v[i].leaf_val_[0])->IsIntersect(ray);
							break;
						}												
						
						if (itsc.distance_>0 && itsc.distance_ < min_dist)
						{	
							min_id = v[i].id_;
							min_dist = itsc.distance_;
							nearest_itsc = itsc;						
						}
					}
					else
					{
						/// insert two child into buffer
						if (v[i].left_ != NULL)
							v.push_back(*v[i].left_);
						if (v[i].right_ != NULL)
							v.push_back(*v[i].right_);
					}					
				}
			}
			// post-deal operation
			v.erase(v.begin(), v.begin() + current_level_num);
			current_depth++;
		}
	}
		
	if (min_id != INT_MAX)
	{
		nearest_itsc.is_hit_ = true;
	}		
	else
	{
		nearest_itsc.is_hit_ = false;
	}

	return nearest_itsc;
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
	if (head != NULL)
	{
		id_record_[head->depth_].push_back(head->id_);
	}
	else
		return;

	GetPrint(head->left_);
	GetPrint(head->right_);
	
}


// if rst==INT_MAX, then abnormal
// else ,normal 
float  KdTree::GetMean(vector<Object*>& dat, int axis)
{
	float avr = 0;
	float accumulator = 0;

	switch (axis)
	{
	case 0:
		avr = dat[0]->center_.x;
		for (int i = 1; i < dat.size(); i++)
		{
			avr += dat[i]->center_.x;
			accumulator += abs(dat[i - 1]->center_.x - dat[i]->center_.x);
		}
		break;
	case 1:
		avr = dat[0]->center_.y;
		for (int i = 1; i < dat.size(); i++)
		{
			avr += dat[i]->center_.y;
			accumulator += abs(dat[i - 1]->center_.y - dat[i]->center_.y);
		}
		break;
	case 2:
		avr = dat[0]->center_.z;
		for (int i = 1; i < dat.size(); i++)
		{
			avr += dat[i]->center_.z;
			accumulator += abs(dat[i - 1]->center_.z - dat[i]->center_.z);
		}
		break;
	}

	if (accumulator < 0.00001)
		return INT_MAX;
	else
	{
		avr = avr / dat.size();
		return avr;
	}	
}