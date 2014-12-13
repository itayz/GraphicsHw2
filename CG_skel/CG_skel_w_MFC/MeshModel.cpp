#include "StdAfx.h"
#include "MeshModel.h"
#include "vec.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

struct FaceIdcs
{
	int v[4];
	int vn[4];
	int vt[4];

	FaceIdcs()
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream & aStream)
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for(int i = 0; i < 3; i++)
		{
			aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};

vec3 vec3fFromStream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

vec2 vec2fFromStream(std::istream & aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}

MeshModel::MeshModel(string fileName, bool uniform)
{
	loadFile(fileName);
}

MeshModel::~MeshModel(void)
{
}

void MeshModel::loadFile(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> faces;
	vector<vec3> _vertices;
	vector<vec3> _normals;
	vector<vec2> _texture;
	bounds_min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);//bounding box of our model.
	bounds_max = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);//bounding box of our model.


	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

		// read type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v") { //vertex
			_vertices.push_back(vec3fFromStream(issLine));
			for (int i = 0; i<3; i++) { //while vertices are added we calculate bounding box.
				bounds_min[i] = min(bounds_min[i], _vertices.at(_vertices.size()-1)[i]);
				bounds_max[i] = max(bounds_max[i], _vertices.at(_vertices.size()- 1)[i]);
			}
		}
		else if (lineType == "vn") //vertex normal
			_normals.push_back(vec3fFromStream(issLine));
		else if (lineType == "vt") //vertex texture
			_texture.push_back(vec2fFromStream(issLine));
		else if (lineType == "f") //face
			faces.push_back(issLine);
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			cout<< "Found unknown line Type \"" << lineType << "\"";
		}
	}
	//vertices is vector of vec3. Every three elements define a triangle in 3D.
	//If the face part of the obj is
	//f 1 2 3
	//f 1 3 4
	//Then vertex_positions should contain:
	//vertex_positions={v1,v2,v3,v1,v3,v4}
	vertices.reserve(3 * faces.size()); //allocate space.
	// iterate through all stored faces and create triangles
	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
	{
		
		for (int i = 0; i < 3; i++)
		{
			// add vertex positions
			vertices.push_back(vec3(_vertices[it->v[i] - 1])); //OBJ format indice start from 1 unlike our vector structure which starts from 0.

			// add vertex normals.  we assume data is consistent in way user reference the vertex data. OBJ format.
			if (it->vn[i] != 0) {
				vertex_normals.push_back(vec3(normalize(_normals[it->vn[i] - 1])));
			}
			//add vertex textures.
			if (it->vt[i] != 0) {
				vertex_texture.push_back(vec2(normalize(_texture[it->vt[i] - 1])));
			}
		}
	}
	bounding_box.push_back(bounds_min);
	bounding_box.push_back(bounds_max);
	compute_face_normals();
}



void MeshModel::draw(Renderer& renderer, COLORS color)
{
	renderer.SetObjectMatrices(_world_transform, _normal_transform);
	renderer.DrawTriangles(&vertices, NULL, NULL, color, &material); //draw only triangles. default case.
	if (renderer.draw_triangle_normals)
		renderer.DrawFaceNormals(&face_normals);
	if (renderer.draw_vertex_normals)
		renderer.DrawVertexNormals(&vertices, &vertex_normals);
	if (renderer.draw_bounding_box)
		renderer.DrawBoundingBox(&bounding_box);
}


void MeshModel::compute_face_normals() {
	face_normals.clear(); //init
	for (int i = 0; i < vertices.size(); i += 3) {
		vec3 tail = (1.0f / 3) * (vertices[i] + vertices[i + 1] + vertices[i + 2]); //vector to the center of the triangle from datum point.
		face_normals.push_back(tail);
		vec3 normal = normalize(cross((vertices[i + 1] - vertices[i]), (vertices[i + 2] - vertices[i]))); //scaled triangle normal.
		face_normals.push_back(tail + normal); //normal to triangle from the datum point.
	}
}

void MeshModel::setModelMaterial(const ModelMaterial& m)
{
	material = m;
}