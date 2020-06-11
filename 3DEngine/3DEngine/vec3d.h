/*
Function Definitions for 3D Vector Calculations
Written By: Marcelo Schonbrunn Bonnin
*/

#pragma once
#include "Structs.h"

//Functions for Normalizing
vec3d calculateNormalLine(triangle t, int point);
vec3d V_Normalise(vec3d &v);
vec3d normalize(vec3d vec);
float Vector_Length(vec3d &v);

//Functions for Vector Operations
vec3d V_Add(vec3d &vec1, vec3d &vec2);
vec3d V_Subtract(vec3d &vec1, vec3d &vec2);
vec3d V_Divide(vec3d &vec1, float k);

//Functions for Vector Products
float V_DotProduct(vec3d &vec1, vec3d &vec2);
float dotProduct(vec3d vec, triangle t, vec3d camera);
vec3d V_CrossProduct(vec3d &vec1, vec3d &vec2);