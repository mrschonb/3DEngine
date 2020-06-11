/*
Function Definitions for 4D matrix Calculations
Written by: Marcelo Schonbrunn Bonnin
*/

#pragma once
#include "Structs.h"

void MultVectorMatrix(vec3d &in, vec3d &out, mat4d &m);

//Matrix Creation Functions
mat4d M_MakeIdentity();
mat4d M_RotationZ(float fAngleRad);
mat4d M_RotationX(float fAngleRad);
mat4d M_MakeRotationY(float fAngleRad);
mat4d M_MakeTranslation(float x, float y, float z);

//Matrix Handling Functions
mat4d M_Projection(float fFovDegrees, float fAspectRatio, float fNear, float fFar);

//Matrix Operation Functions
mat4d M_MultiplyMatrix(mat4d &m1, mat4d &m2);
vec3d M_MultiplyVector(mat4d &m, vec3d &i);
