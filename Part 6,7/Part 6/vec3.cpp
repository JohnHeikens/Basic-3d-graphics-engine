#include "vec3.h"
bool operator !=(vec3 v1, vec3 v2)
{
	return v1.x != v2.x || v1.y != v2.y || v1.z != v2.z;
}
bool operator ==(vec3 v1, vec3 v2)
{
	return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
}
vec3 operator *(vec3 v, float scale)
{
	return vec3(v.x * scale, v.y * scale, v.z * scale);
}
vec3 operator *(float scale, vec3 v)
{
	return vec3(v.x * scale, v.y * scale, v.z * scale);
}
vec3 operator /(vec3 v, float scale)
{
	return vec3(v.x / scale, v.y / scale, v.z / scale);
}
vec3 operator +(vec3 v1, vec3 v2)
{
	return vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
vec3 operator +(vec3 v1, float additional)
{
	return vec3(v1.x + additional, v1.y + additional, v1.z + additional);
}
vec3 operator -(vec3 v1, vec3 v2)
{
	return vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
vec3 operator -(vec3 v)
{
	return vec3(-v.x, -v.y, -v.z);
}
vec3::vec3()
{
	x = y = z = 0;
}
vec3::vec3(fp x, fp y, fp z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
vec3::vec3(fp x, fp y)
{
	this->x = x;
	this->y = y;
	this->z = 0;
}
vec3::vec3(fp scale)
{
	x = y = z = scale;
}
fp vec3::lengthsquared()
{
	return x * x + y * y + z * z;
}
fp vec3::length()
{
	return sqrt(lengthsquared());
}
void vec3::Normalize()
{
	fp l = length();
	x /= l;
	y /= l;
	z /= l;
}
