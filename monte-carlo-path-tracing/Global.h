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
#include "Material.hpp"
#define LAMBERTIAN_SAMPLE_NUMBER 4.0f
#define NUMBER_OF_LIGHT_SAMPLES 4.0f
#define PI 3.1415926535897f
#define BLINN_PHONG_SAMPLE_NUMBER 10.0f
#define BLINN_PHONG_MAX_DEPTH 6
// image size 
#define IMAGE_WIDTH	512
#define IMAGE_HEIGHT 512

// variables
extern vector<V3> v_;		// vertex load from obj file
extern vector<V3> vt_;		// vertex texture load from obj file
extern vector<V3> vn_;		// vertex normal load from obj file
extern vector<Material> mtls_; // material load from mtl file
enum ObjectType { PATCH, SPHERE_SOURCE, QUAD_SOURCE };
extern V3 background;
extern ofstream global_file;
extern float buf_temp[IMAGE_WIDTH*IMAGE_HEIGHT];

// function
V3 GetRandom();
float GetRandomFloat();
void remove_adjacent_duplicate(string& dat1, string dat2);
vector<string> split(string dat, string separator);




