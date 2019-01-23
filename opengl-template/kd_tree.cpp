#include "kd_tree.h"
KdTree kdt1(3);
void KdTree::insert(vector<float> &dat)
{
	tree_.insert(tree_.end(), dat.begin(), dat.end());

}
KdTree::KdTree(int n)
{
	dimension_ = n;
}

void KdTree::build()
{

}
int KdTree::get_level(int pos)
{




}