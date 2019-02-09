#pragma once
#include <vector>
#include "V3.hpp"
using namespace std;

template <class T>
class KdNode
{
public:
	int id_;

};


template <class T>
class KdTree
{
public:
	int dimension_;
	vector<T*> raw_;
	vector<T> tree_;

	KdTree(int n);

	void Init();
	void Insert(T& dat);
	void Build();
	void SearchNearstNeighbor();
	int GetLevel(int pos);
};