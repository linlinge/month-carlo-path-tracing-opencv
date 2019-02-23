#include "Material.h"


// Material Function Definition
Material& Material::operator=(Material& dat)
{
	name_ = dat.name_;
	illum_ = dat.illum_;
	Kd_ = dat.Kd_;
	Ka_ = dat.Ka_;
	Tf_ = dat.Tf_;
	Ks_ = dat.Ks_;
	Ni_ = dat.Ni_;
	Ns_ = dat.Ns_;
	return *this;
}

void Material::Clear()
{
	name_.clear();
	illum_ = INT_MAX;
	Kd_.Clear();
	Ka_.Clear();
	Tf_.Clear();
	Ks_.Clear();
	Ni_ = 0;
	Ns_ = 0;
}
