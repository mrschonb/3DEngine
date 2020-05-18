// 3DEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "olcConsoleGameEngine.h"

struct vec3d
{
	float x, y, z;
};

struct triangle 
{
	vec3d p[3];
};

struct mesh
{
	std::vector<triangle> tris;
};

struct mat4d
{
	float m[4][4] = {0};
};

class olcEngine3D : public olcConsoleGameEngine {

private:
	mesh meshCube;
	mat4d projMat;
	float fTheta;

	void MultVectorMatrix(vec3d &in, vec3d &out, mat4d &m)
	{
		out.x = in.x * m.m[0][0] + in.y * m.m[1][0] + in.z * m.m[2][0] + m.m[3][0];
		out.y = in.x * m.m[0][1] + in.y * m.m[1][1] + in.z * m.m[2][1] + m.m[3][1];
		out.z = in.x * m.m[0][2] + in.y * m.m[1][2] + in.z * m.m[2][2] + m.m[3][2];
		float w = in.x * m.m[0][3] + in.y * m.m[1][3] + in.z * m.m[2][3] + m.m[3][3];

		if (w != 0)
		{
			out.x /= w; out.y /= w; out.z /= w;
		}
	}

public:
	olcEngine3D() {
		m_sAppName = L"3D Engine Test";
		fTheta = 0;
	}

public:
	bool OnUserCreate() override {

		//define the object for the mesh, in this case a cube. n.
		meshCube.tris = {
			//S face of cube
			{0.0f,0.0f,0.0f,	0.0f,1.0f,0.0f,		1.0f,1.0f,0.0f},
			{0.0f,0.0f,0.0f,	1.0f,1.0f,0.0f,		1.0f,0.0f,0.0f},
			//E Face of cube
			{1.0f,0.0f,0.0f,	1.0f,1.0f,0.0f,		1.0f,1.0f,1.0f},
			{1.0f,0.0f,0.0f,	1.0f,1.0f,1.0f,		1.0f,0.0f,1.0f},
			//N face of cube
			{1.0f,0.0f,1.0f,	1.0f,1.0f,1.0f,		0.0f,1.0f,1.0f},
			{1.0f,0.0f,1.0f,	0.0f,1.0f,1.0f,		0.0f,0.0f,1.0f},
			//W face of cube
			{0.0f,0.0f,1.0f,	0.0f,1.0f,1.0f,		0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f,	0.0f,1.0f,0.0f,		0.0f,0.0f,0.0f},
			//TOP face of cube
			{0.0f,1.0f,0.0f,	0.0f,1.0f,1.0f,		1.0f,1.0f,1.0f},
			{0.0f,1.0f,0.0f,	1.0f,1.0f,1.0f,		1.0f,1.0f,0.0f},
			//BOTTOM face of cube
			{1.0f,0.0f,1.0f,	0.0f,0.0f,1.0f,		0.0f,0.0f,0.0f},
			{1.0f,0.0f,1.0f,	0.0f,0.0f,0.0f,		1.0f,0.0f,0.0f},
		};  

		//Populate the projection matrix
		float fNear = 0.1f;
		float fFar = 1000.0f;
		float fFov = 90.0f;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovRad = 1.0f / tanf(fFov *0.5f / 180.0f * 3.14159f);

		projMat.m[0][0] = fAspectRatio * fFovRad;
		projMat.m[1][1] = fFovRad;
		projMat.m[2][2] = fFar / (fFar - fNear);
		projMat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		projMat.m[2][3] = 1.0f;
		projMat.m[3][3] = 0.0f;

		return true;
	}

	//calculate normal lines using a triangle 
	vec3d calculateNormalLine(triangle t, int point)
	{
		vec3d line;
		line.x = t.p[point].x - t.p[0].x;
		line.y = t.p[point].y - t.p[0].y;
		line.z = t.p[point].z - t.p[0].z;

		return line;
	}

	bool OnUserUpdate(float fElapsedTime) override{

		//clear the screen with a solid color
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
		mat4d matRotZ, matRotX;
		fTheta += 1.0f * fElapsedTime;


		//Rotational Matrix in the Z direction
		matRotZ.m[0][0] = cosf(fTheta);
		matRotZ.m[0][1] = sinf(fTheta);
		matRotZ.m[1][0] = -sinf(fTheta);
		matRotZ.m[1][1] = cosf(fTheta);
		matRotZ.m[2][2] = 1;
		matRotZ.m[3][3] = 1;

		//Rotational Matrix in the X direction
		matRotX.m[0][0] = 1;
		matRotX.m[1][1] = cosf(fTheta * 0.5f);
		matRotX.m[1][2] = sinf(fTheta * 0.5f);
		matRotX.m[2][1] = -sinf(fTheta * 0.5f);
		matRotX.m[2][2] = cosf(fTheta * 0.5f);
		matRotX.m[3][3] = 1;


		//Draw our poligon using triangle operations
		for (auto tri : meshCube.tris)
		{
			triangle tProj, tTranslated, triRotZ, triRotZX;

			MultVectorMatrix(tri.p[0], triRotZ.p[0], matRotZ);
			MultVectorMatrix(tri.p[1], triRotZ.p[1], matRotZ);
			MultVectorMatrix(tri.p[2], triRotZ.p[2], matRotZ);

			MultVectorMatrix(triRotZ.p[0], triRotZX.p[0], matRotX);
			MultVectorMatrix(triRotZ.p[1], triRotZX.p[1], matRotX);
			MultVectorMatrix(triRotZ.p[2], triRotZX.p[2], matRotX);

			tTranslated = triRotZX;
			tTranslated.p[0].z = triRotZX.p[0].z + 3.0f;
			tTranslated.p[1].z = triRotZX.p[1].z + 3.0f;
			tTranslated.p[2].z = triRotZX.p[2].z + 3.0f;

			//calculating normals for triangles for rendering optimization
			vec3d normal, line1, line2;

			line1 = calculateNormalLine(tTranslated, 1);
			line2 = calculateNormalLine(tTranslated, 2);
			
			//calculate the normal using the previous lines
			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			//normalizing normal by vector translation
			float l = sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
			normal.x /= l; normal.y /= l; normal.z /= l;

			if (normal.z < 0)
			{
				MultVectorMatrix(tTranslated.p[0], tProj.p[0], projMat);
				MultVectorMatrix(tTranslated.p[1], tProj.p[1], projMat);
				MultVectorMatrix(tTranslated.p[2], tProj.p[2], projMat);


				tProj.p[0].x += 1.0f; tProj.p[0].y += 1.0f;
				tProj.p[1].x += 1.0f; tProj.p[1].y += 1.0f;
				tProj.p[2].x += 1.0f; tProj.p[2].y += 1.0f;

				tProj.p[0].x *= 0.5f * (float)ScreenWidth();
				tProj.p[0].y *= 0.5f * (float)ScreenHeight();
				tProj.p[1].x *= 0.5f * (float)ScreenWidth();
				tProj.p[1].y *= 0.5f * (float)ScreenHeight();
				tProj.p[2].x *= 0.5f * (float)ScreenWidth();
				tProj.p[2].y *= 0.5f * (float)ScreenHeight();

				DrawTriangle(tProj.p[0].x, tProj.p[0].y,
				tProj.p[1].x, tProj.p[1].y,
				tProj.p[2].x, tProj.p[2].y, PIXEL_SOLID, FG_WHITE);
			}
		}
		return true;
	}

};

int main()
{

	olcEngine3D demo;
	if (demo.ConstructConsole(256, 240, 4, 4))
		demo.Start();
	else {
		perror("Could not open console");
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
