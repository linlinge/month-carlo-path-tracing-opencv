#include "LoadObjFile.h"

void ObjFile::LoadObjs(string filename)
{
	ifstream f(filename);
	string line;
	int flag = 0;
	int temp_mtl_id=INT_MAX;
	int temp_obj_name_id=INT_MAX;

	while (getline(f, line))
	{
		remove_adjacent_duplicate(line, " ");
		vector<string> ss = split(line, " ");
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
			v_.push_back(temp);
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
			vt_.push_back(temp);
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
			vn_.push_back(temp);
		}
		else if (ss[0] == "f")
		{
			Patch f_temp;
			vector<V3> p_temp;
			for (int i = 1; i < ss.size(); i++)
			{
				vector<string> temp = split(ss[i], "/");
				switch (temp.size())
				{
				case 3:
					f_temp.vn_id_.push_back(atoi(temp[2].c_str()) - 1);
				case 2:
					f_temp.vt_id_.push_back(atoi(temp[1].c_str()) - 1);
				case 1:
					int id_temp = atoi(temp[0].c_str()) - 1;
					f_temp.v_id_.push_back(id_temp);

					p_temp.push_back(v_[id_temp]);
				}
			}

			// caculate normal
			V3 v1 = p_temp[0] - p_temp[1];
			V3 v2 = p_temp[1] - p_temp[2];
			V3 v3 = Cross(v1, v2);
			f_temp.normal_ = v3.GetNorm();
			
			// store obj_name_id_	
			if(temp_obj_name_id!=INT_MAX)
				f_temp.obj_name_id_ = temp_obj_name_id;

			if(temp_mtl_id!=INT_MAX)
				f_temp.mtl_id_ = temp_mtl_id;

			// store Patch
			f_.push_back(f_temp);
		}
		else if (ss[0] == "g")
		{
			if (ss[1] != "default")
			{
				temp_obj_name_id = objs_name_.size();
				objs_name_.push_back(ss[1]);
			}			
		}
		else if (ss[0] == "usemtl")
		{
			temp_mtl_id = GetMtlId(ss[1]);			
		}
		else if (ss[0] == "mtllib")
		{
			// find last
			int pos = filename.size() - 1;
			while (filename[pos] != '/') pos--;
			string str = filename.substr(0, pos + 1) + ss[1];
			LoadMaterial(str);
		}
	}

	// get properties( center_ )
	GetProperties();
}

void ObjFile::LoadMaterial(string filename)
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
					mtls_.push_back(mtl_temp);
					mtl_temp.Clear();
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
		mtls_.push_back(mtl_temp);
	}
	else
	{
		cout << "No material file found!" << endl;
	}
}

int ObjFile::GetMtlId(string str)
{
	for (int i = 0; i < mtls_.size(); i++)
	{
		if (str == mtls_[i].name_)
			return i;
	}

	return -1;
}

void ObjFile::Clear()
{
	v_.clear();
	vt_.clear();
	vn_.clear();
	mtls_.clear();
	objs_name_.clear();
	f_.clear();
}

void ObjFile::GetProperties()
{
	for (auto& f_temp: f_)
	{
		// get bound box
		/// init 
		f_temp.box_.top_left_ = f_temp.box_.bottom_right_ = v_[f_temp.v_id_[0]];
		/// expand
		for (int i = 1; i < f_temp.v_id_.size(); i++)
		{
			f_temp.box_.Expand(v_[f_temp.v_id_[i]]);
			f_temp.center_ = f_temp.center_ + v_[f_temp.v_id_[i]];
		}
		// get center
		f_temp.center_ = f_temp.center_ / f_temp.v_id_.size();
	}
}