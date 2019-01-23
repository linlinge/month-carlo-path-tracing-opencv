#pragma once
#include <vector>
using namespace std;


class KdTree
{
public:
	int dimension_;
	vector<float> tree_;

	KdTree(int n);
	void insert(vector<float> &dat);
	void build();
	void search_nearst_neighbor();
	int get_level(int pos);
};

extern KdTree kdt1(3);