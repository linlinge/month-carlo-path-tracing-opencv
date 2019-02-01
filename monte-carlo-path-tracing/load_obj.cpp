#include "load_obj.h"
vector<V3> g_v;
vector<V3> g_vt;
vector<V3> g_vn;
vector<Material> g_mtls;

void LoadObj(string filename, vector<Obj>& dat)
{
	ifstream f(filename);
	string line;
	int flag = 0;
	Obj obj_temp;
	while (getline(f, line))
	{
		remove_adjacent_duplicate(line," ");
		vector<string> ss = split(line," ");
		if (ss[0] == "v")
		{
			V3 temp;
			switch (ss.size())
			{
			case 4:
				temp.z = atof(ss[3].c_str());
			case 3:
				temp.y = atof(ss[2].c_str());
			case 2:
				temp.x = atof(ss[1].c_str());
			}
			g_v.push_back(temp);
		}
		else if (ss[0] == "vt")
		{
			V3 temp;
			switch (ss.size())
			{
			case 4:
				temp.z = atof(ss[3].c_str());
			case 3:
				temp.y = atof(ss[2].c_str());
			case 2:
				temp.x = atof(ss[1].c_str());
			}
			g_vt.push_back(temp);
		}
		else if (ss[0] == "vn")
		{
			V3 temp;
			switch (ss.size())
			{
			case 4:
				temp.z = atof(ss[3].c_str());
			case 3:
				temp.y = atof(ss[2].c_str());
			case 2:
				temp.x = atof(ss[1].c_str());
			}
			g_vn.push_back(temp);
		}
		else if (ss[0] == "f")
		{
			face f_temp;
			vector<V3> p_temp;
			for (int i = 1; i < ss.size(); i++)
			{
				vector<string> temp = split(ss[i],"/");
				switch(temp.size())
				{
				case 3:
					f_temp.vn_id_.push_back(atoi(temp[2].c_str())-1);
				case 2:
					f_temp.vt_id_.push_back(atoi(temp[1].c_str())-1);
				case 1:
					int id_temp = atoi(temp[0].c_str()) - 1;
					f_temp.v_id_.push_back(id_temp);

					p_temp.push_back(g_v[id_temp]);
				}
			}
			// caculate normal
			V3 v1 = p_temp[0] - p_temp[1];
			V3 v2 = p_temp[1] - p_temp[2];
			V3 v3 = v1.Cross(v2);
			f_temp.normal_=v3.GetNorm();

			// calculate box
			float min_x, min_y, min_z;
			float max_x, max_y, max_z;
			min_x = min_y = min_z = INT_MAX;
			max_x = max_y = max_z = -INT_MAX;

			for (auto& f_id : f_temp.v_id_)
			{
				V3& v = g_v[f_id];				
				min_x = min_x < v.x ? min_x : v.x;
				min_y = min_y < v.y ? min_y : v.y;
				min_z = min_z < v.z ? min_z : v.z;

				max_x = max_x > v.x ? max_x : v.x;
				max_y = max_y > v.y ? max_y : v.y;
				max_z = max_z > v.z ? max_z : v.z;
			}
			f_temp.box_.min_ = V3(min_x, min_y, min_z);
			f_temp.box_.max_ = V3(max_x, max_y, max_z);

			// store face
			obj_temp.f_.push_back(f_temp);
		}
		else if (ss[0] == "g")
		{
			if (ss[1] == "default")
			{
				if (flag != 0)
				{
					dat.push_back(obj_temp);
					obj_temp.clear();
				}
				flag = 1;
			}
			else
			{
				obj_temp.obj_name_ = ss[1];
			}
		}
		else if (ss[0] == "usemtl")
		{
			obj_temp.usemtl_ = ss[1];
			obj_temp.mtl_id_ = obj_temp.get_mtl_id(ss[1]);
		}
		else if (ss[0] == "s")
		{
			SmoothGroup sg_temp;
			sg_temp.status_ = atoi(ss[1].c_str());
			sg_temp.position_ = obj_temp.f_.size()+1;
			obj_temp.s_.push_back(sg_temp);
		}
		else if (ss[0]=="mtllib")
		{
			// find last
			int pos = filename.size() - 1;
			while (filename[pos] != '/') pos--;
			string str = filename.substr(0,  pos+1) + ss[1];
			LoadMaterial(str);
		}
	}
	dat.push_back(obj_temp);
}

void LoadMaterial(string filename)
{
	ifstream f;
	f.open(filename);
	if (f.is_open() == true)
	{
		string line;
		int flag = 0;
		Material mtl_temp;
		while (getline(f, line))
		{
			remove_adjacent_duplicate(line, " ");
			vector<string> ss = split(line, " ");
			if (ss[0] == "newmtl")
			{
				if (flag != 0)
				{
					g_mtls.push_back(mtl_temp);
					mtl_temp.clear();
				}
				mtl_temp.name_ = ss[1];
				flag++;
			}
			else if (ss[0] == "illum")
			{
				mtl_temp.illum_ = atoi(ss[1].c_str());
			}
			else if (ss[0] == "Kd")
			{
				mtl_temp.Kd_.x = atof(ss[1].c_str());
				mtl_temp.Kd_.y = atof(ss[2].c_str());
				mtl_temp.Kd_.z = atof(ss[3].c_str());
			}
			else if (ss[0] == "Ka")
			{
				mtl_temp.Ka_.x = atof(ss[1].c_str());
				mtl_temp.Ka_.y = atof(ss[2].c_str());
				mtl_temp.Ka_.z = atof(ss[3].c_str());
			}
			else if (ss[0] == "Tf")
			{
				mtl_temp.Tf_.x = atof(ss[1].c_str());
				mtl_temp.Tf_.y = atof(ss[2].c_str());
				mtl_temp.Tf_.z = atof(ss[3].c_str());
			}
			else if (ss[0] == "Ni")
			{
				mtl_temp.Ni_ = atoi(ss[1].c_str());
			}
			else if (ss[0] == "Ks")
			{
				mtl_temp.Ks_.x = atof(ss[1].c_str());
				mtl_temp.Ks_.y = atof(ss[2].c_str());
				mtl_temp.Ks_.z = atof(ss[3].c_str());
			}
			else if (ss[0] == "Ns")
			{
				mtl_temp.Ns_ = atoi(ss[1].c_str());
			}
		}
		g_mtls.push_back(mtl_temp);
	}
	else
	{
		cout << "No material file found!" << endl;
	}	
}

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


