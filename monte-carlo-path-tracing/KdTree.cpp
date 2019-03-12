#include "KdTree.h"
#include <fstream>
#include <iostream>
using namespace std;
int number = 0;

KdNode* KdTree::Build(vector<Object*>& objs,int depth)
{
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
		max_box.Expand(objs[i]->center_);
	}
	node->box_ = max_box;

	// leaf node or not
	if (depth > MAX_DEPTH || objs.size()==1)
	{
		node->leaf_val_ = objs[0];
		return node;
	}

	
	// get the axis
	node->axis_ = node->box_.GetMaxAxis();

	// splite data
	/// get middle value
	node->internal_val_ = 0;
	for (int i = 0; i < objs.size(); i++)
	{
		switch (node->axis_)
		{
		case 0:
			node->internal_val_ += objs[i]->center_.x;
			break;
		case 1:
			node->internal_val_ += objs[i]->center_.y;
			break;
		case 2:
			node->internal_val_ += objs[i]->center_.z;
			break;
		}
	}
	node->internal_val_ = node->internal_val_ / objs.size();

	/// 
	vector<Object*> left, right;	
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

Patch KdTree::NearestSearch(Ray& ray)
{
	KdNode node_temp;
	stack<KdNode> s;
	float min_dist = INT_MAX;
	Patch nearest_patch;

	if (root_->box_.IsIntersect(ray) != true)
		return Patch();
	else
		cout << "Has Intersection" << endl;

	s.push(*root_);
	if (root_->right_ != NULL)
		s.push(*root_->right_);

	if (root_->left_ != NULL)
		s.push(*root_->left_);

	do
	{	// left child
		if (s.top().left_ == NULL  || *s.top().left_ == node_temp)// have been detected
			/// left child has been detected
		{					
			if (s.top().right_ != NULL)
			{
				/// output middle	
				cout << s.top().id_ << endl;

				/// push right child			
				s.push(*s.top().right_); 
				/// may be it still has children, so need to output
			}
			else
				/// leaf node
			{		
				cout << s.top().id_ << endl;
				node_temp = s.top();
				s.pop();
			}			
		}
		else if (s.top().right_==NULL || *s.top().right_==node_temp)
		{			
			node_temp = s.top();
			s.pop();
		}
		else
			/// detect left child
		{
			while(s.top().left_!=NULL ) s.push(*s.top().left_);			
		}		
	} while (s.top() != *root_);

	cout << root_->id_ << endl;

	return nearest_patch;
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
						Intersection itsc = v[i].leaf_val_[0].IsIntersect(ray);
						
						if (itsc.distance_>0 && itsc.distance_ < min_dist)
						{	
							min_id = v[i].id_;
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


KdNode* KdTree::NearestSearchRecursive(KdNode* node)
{

	if (node)
	{
		NearestSearchRecursive(node->left_);
		cout << node->id_ << endl;
		NearestSearchRecursive(node->right_);
	}
	return NULL;
	
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