/*
Function Implementations for 3D Vector Calculations
Written By: Marcelo Schonbrunn Bonnin
*/

#include "vec3d.h"
#include <algorithm>
vec3d calculateNormalLine(triangle t, int point)
{
	vec3d line;
	line.x = t.p[point].x - t.p[0].x;
	line.y = t.p[point].y - t.p[0].y;
	line.z = t.p[point].z - t.p[0].z;

	return line;
}

vec3d V_Add(vec3d &vec1, vec3d &vec2)
{
return { vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z };
}

vec3d V_Subtract(vec3d &vec1, vec3d &vec2)
{
return { vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z };
}

vec3d V_Divide(vec3d &vec1, float scalar)
{
return { vec1.x / scalar, vec1.y / scalar, vec1.z / scalar };
}

float V_DotProduct(vec3d &vec1, vec3d &vec2)
{
return vec1.x*vec2.x + vec1.y*vec2.y + vec1.z * vec2.z;
}

float Vector_Length(vec3d &v)
{
return sqrtf(V_DotProduct(v, v));
}

vec3d V_Normalise(vec3d &v)
{
float l = Vector_Length(v);
return { v.x / l, v.y / l, v.z / l };
}


vec3d normalize(vec3d vec)
{
float l = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
vec.x /= l; vec.y /= l; vec.z /= l;
return vec;
}

float dotProduct(vec3d vec, triangle t, vec3d camera)
{
return (vec.x * (t.p[0].x - camera.x) +
	vec.y * (t.p[0].y - camera.y) +
	vec.z * (t.p[0].z - camera.z));
}

vec3d V_CrossProduct(vec3d &vec1, vec3d &vec2)
{
	vec3d v;
	v.x = vec1.y * vec2.z - vec1.z * vec2.y;
	v.y = vec1.z * vec2.x - vec1.x * vec2.z;
	v.z = vec1.x * vec2.y - vec1.y * vec2.x;
	return v;
}