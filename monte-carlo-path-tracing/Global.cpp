#include "global.h"
vector<V3> v_;
vector<V3> vt_;
vector<V3> vn_;
vector<Material> mtls_;
V3 background(10,10,10);

ofstream global_file("../output/hit_patch.txt");

// get random normlized vector in hemisphere
V3 GetRandom()
{
	float x_temp, y_temp, z_temp;
	float xxyy_temp = INT_MAX;
	while (xxyy_temp > 1)
	{
		srand((unsigned)time(NULL));
		x_temp = rand() / double(RAND_MAX);
		y_temp = rand() / double(RAND_MAX);
		xxyy_temp = pow(x_temp, 2) + pow(y_temp, 2);
	}
	z_temp = sqrt(1 - xxyy_temp);

	return V3(x_temp, y_temp, z_temp);
}


// remove adjacent duplicate substring in string
void remove_adjacent_duplicate(string& dat1, string dat2)
{
	vector<int> pos;
	int cur = dat1.find(dat2, 0);
	if (cur != -1)
	{
		while (cur != -1)
		{
			if (cur != -1)
				pos.push_back(cur);
			cur = dat1.find(dat2, cur + 1);
		}

		// remove duplicate
		int adjust = 0;
		for (int i = 0; i < pos.size() - 1; i++)
		{
			if (pos[i] + 1 == pos[i + 1])
			{
				dat1.erase(pos[i] + adjust, 1);
				adjust--;
			}
		}
	}
}

vector<string> split(string dat, string separator)
{
	vector<string> rst;
	int start, end;
	do
	{
		start = dat.find(separator);
		end = start + separator.size();
		rst.push_back(dat.substr(0, start));
		dat.erase(0, end);
	} while (start != -1);
	return rst;
}

