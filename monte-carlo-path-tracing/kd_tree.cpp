#include "kd_tree.h"
template <class T>
void KdTree<T>::Insert(T& dat)
{
	raw_.push_back(&dat);
}

template <class T>
KdTree<T>::KdTree(int n)
{
	dimension_ = n;
}

template <class T>
void KdTree<T>::Build()
{

}

template <class T>
void KdTree<T>::SearchNearstNeighbor()
{

}

template <class T>
int KdTree<T>::GetLevel(int pos)
{

	return 1;
}
