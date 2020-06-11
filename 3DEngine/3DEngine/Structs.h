/*
Definition of 3D / 4D struct Types
Written by: Marcelo Schonbrunn Bonnin
*/
#pragma once
#include <vector>
#include <fstream>
#include <sstream>
struct vec3d
{
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

struct triangle
{
	vec3d p[3];
	wchar_t sym;
	short color;
};

struct mat4d
{
	float m[4][4] = { 0 };
};

struct mesh
{
	std::vector<triangle> triangles;

	bool loadOBJ(std::string fname)
	{
		std::ifstream file(fname);
		if (!file.is_open()) { return false; }

		std::vector<vec3d> vertexes;
		char *line = new char[128];
		while (!file.eof())
		{
			file.getline(line, 128);

			std::stringstream s;
			s << line;

			char type;

			if (line[0] == 'v')
			{
				vec3d v;
				s >> type >> v.x >> v.y >> v.z;
				vertexes.push_back(v);
			}

			if (line[0] == 'f')
			{
				int f[3];
				s >> type >> f[0] >> f[1] >> f[2];
				triangles.push_back({ vertexes[f[0] - 1], vertexes[f[1] - 1] ,vertexes[f[2] - 1] });

			}
			/*for (auto t : triangles)
			{
				printf("%f %f %f", t.p[0].x, t.p[0].y, t.p[0].z);
				printf("\n");
			}*/
		}
		delete(line);
		return true;
	}
};
