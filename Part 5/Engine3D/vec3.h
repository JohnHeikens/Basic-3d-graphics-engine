#include "GlobalFunctions.h"
#pragma once
struct vec3
{
public:
	fp x, y, z;
	vec3();
	vec3(fp x, fp y, fp z);
	vec3(fp x, fp y);
	vec3(fp scale);
	fp lengthsquared();
	fp length();
	void Normalize();
};
static bool operator !=(vec3 v1, vec3 v2);
static bool operator ==(vec3 v1, vec3 v2);
static vec3 operator *(vec3 v, float scale);
static vec3 operator *(float scale, vec3 v);
static vec3 operator /(vec3 v, float scale) ;
static vec3 operator +(vec3 v1, vec3 v2);
static vec3 operator +(vec3 v1, float additional);
static vec3 operator -(vec3 v1, vec3 v2) ;
static vec3 operator -(vec3 v);
