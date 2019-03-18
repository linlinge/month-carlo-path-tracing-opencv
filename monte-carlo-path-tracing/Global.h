#pragma once
#include <ctime>
#include <vector>
#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "V3.hpp"
#include "Material.h"
#define LAMBERTIAN_SAMPLE_NUMBER 4.0f
#define PI 3.1415926535897

// variables
extern vector<V3> v_;		// vertex load from obj file
extern vector<V3> vt_;		// vertex texture load from obj file
extern vector<V3> vn_;		// vertex normal load from obj file
extern vector<Material> mtls_; // material load from mtl file
enum ObjectType { PATCH, SPHERE_SOURCE, QUAD_SOURCE };
extern V3 background;

// function
V3 GetRandom();
void remove_adjacent_duplicate(string& dat1, string dat2);
vector<string> split(string dat, string separator);




