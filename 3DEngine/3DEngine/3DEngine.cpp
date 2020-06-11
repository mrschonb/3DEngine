// 3DEngine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
This is a basic implementation of an engine capable of displaying 
3D objects in an graphic environment

In this particular instance, the windows Console is used as the graphic environment.

Functions related to drawing on screen have all been taken from the 3DLib.h file
Which in itself is a modified version of the olcConsoleGameEngine.h file
Information regarding the original file can be found at the top of the 3DLib.h File

As of 10/06/2020 this file is still not in its final version and may be modified
in the future, Source Code and version history can be found in github.
Github Repo: https://github.com/mrschonb/3DEngine


Code Written by: Marcelo Schonbrunn Bonnin

*/

#include <iostream>
#include <algorithm>

#include "3DLib.h"
#include "mat4d.h"
#include "vec3d.h"


class olcEngine3D : public olcConsoleGameEngine {

private:
	mesh meshCube;
	mat4d projMat;
	float Theta;

	vec3d camera;

public:
	olcEngine3D() {
		m_sAppName = L"3D Engine Test Console Engine Provided by @javidx9";
		Theta = 0;
	}

public:
	bool OnUserCreate() override {
		//Load ObjFile
		if (!meshCube.loadOBJ("D:\\3D Objects\\LP_Cow.obj")) {exit(1);}

		//Populate the projection matrix
		projMat = M_Projection(90.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f);

		return true;
	}
	

	bool OnUserUpdate(float fElapsedTime) override{
		//clear the screen with a solid color
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);
		Theta += 1.0f * fElapsedTime;



		//Rotational Matrix in the Z direction
		mat4d matRotZ = M_RotationZ(Theta * 0.5f);

		//Rotational Matrix in the X direction
		mat4d matRotX = M_RotationX(Theta);
		mat4d trans_Matrix = M_MakeTranslation(0.0f, 0.0f, 4.0f);

		mat4d univ_Matrix;
		univ_Matrix = M_MakeIdentity();
		univ_Matrix = M_MultiplyMatrix(matRotZ, matRotX);
		univ_Matrix = M_MultiplyMatrix(univ_Matrix, trans_Matrix);

		std::vector<triangle>raster;
		 
		//Draw our poligon using triangle operations
		for (auto t : meshCube.triangles)
		{
			triangle tProj, tTransform;

			tTransform.p[0] = M_MultiplyVector(univ_Matrix, t.p[0]);
			tTransform.p[1] = M_MultiplyVector(univ_Matrix, t.p[1]);
			tTransform.p[2] = M_MultiplyVector(univ_Matrix, t.p[2]);

			// Calculate triangle Normal
			vec3d normal, line1, line2;

			// Get lines either side of triangle
			line1 = V_Subtract(tTransform.p[1], tTransform.p[0]);
			line2 = V_Subtract(tTransform.p[2], tTransform.p[0]);

			// Take cross product of lines to get normal with regards to triangle
			normal = V_CrossProduct(line1, line2);
			normal = V_Normalise(normal);
	
			vec3d ray = V_Subtract(tTransform.p[0], camera);

			// Check triangle visibility through basic ray casting
			if (V_DotProduct(normal, ray) < 0.0f)
			{

				// Create light source for point in 3D space
				vec3d light = { 0.0f, 1.0f, -1.0f };
				light = V_Normalise(light);

				// check lighting of our polis
				float dp = max(0.1f, V_DotProduct(light, normal));

				CHAR_INFO c = GetColour(dp);
				tTransform.color = c.Attributes;
				tTransform.sym = c.Char.UnicodeChar;

				//Project 3D space to 2D space
				tProj.p[0] = M_MultiplyVector(projMat, tTransform.p[0]);
				tProj.p[1] = M_MultiplyVector(projMat, tTransform.p[1]);
				tProj.p[2] = M_MultiplyVector(projMat, tTransform.p[2]);

				tProj.color = c.Attributes;
				tProj.sym = c.Char.UnicodeChar;

				tProj.p[0] = V_Divide(tProj.p[0], tProj.p[0].w);
				tProj.p[1] = V_Divide(tProj.p[1], tProj.p[1].w);
				tProj.p[2] = V_Divide(tProj.p[2], tProj.p[2].w);


				/*tProj.p[0].x += 1.0f; tProj.p[0].y += 1.0f;
				tProj.p[1].x += 1.0f; tProj.p[1].y += 1.0f;
				tProj.p[2].x += 1.0f; tProj.p[2].y += 1.0f;*/

				vec3d offset = { 1,1,0 };
				tProj.p[0] = V_Add(tProj.p[0], offset);
				tProj.p[1] = V_Add(tProj.p[1], offset);
				tProj.p[2] = V_Add(tProj.p[2], offset);


				tProj.p[0].x *= 0.5f * (float)ScreenWidth();
				tProj.p[0].y *= 0.5f * (float)ScreenHeight();
				tProj.p[1].x *= 0.5f * (float)ScreenWidth();
				tProj.p[1].y *= 0.5f * (float)ScreenHeight();
				tProj.p[2].x *= 0.5f * (float)ScreenWidth();
				tProj.p[2].y *= 0.5f * (float)ScreenHeight();

				raster.push_back(tProj);
			}
		}
		//sort triangles to draw using z axis as index
		sort(raster.begin(), raster.end(), [](triangle &t1, triangle &t2)
		{
			float zcoord1 = (t1.p[0].z + t1.p[1].z + t1.p[2].z) / 3.0f;
			float zcoord2 = (t2.p[0].z + t2.p[1].z + t2.p[2].z) / 3.0f;
			return zcoord1 > zcoord2;

		});
		for (auto &tProj : raster)
		{
			FillTriangle(tProj.p[0].x, tProj.p[0].y,
					tProj.p[1].x, tProj.p[1].y,
					tProj.p[2].x, tProj.p[2].y, tProj.sym, tProj.color);

			/*DrawTriangle(tProj.p[0].x, tProj.p[0].y,
				tProj.p[1].x, tProj.p[1].y,
				tProj.p[2].x, tProj.p[2].y, PIXEL_SOLID, FG_WHITE);*/
		}
		return true;
	}

};

int main()
{

	olcEngine3D console;
	if (console.ConstructConsole(256, 240, 4, 4))
		console.Start();
	else {
		perror("Error: ConstructConsole");
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
