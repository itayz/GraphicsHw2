
#include "stdafx.h"
#include "PrimMeshModel.h"

PrimMeshModel::PrimMeshModel(int xMin, int xMax, int zMin, int zMax) : primitive_type(XZ_GRID)
{
	//vec3 p(xMin, 0, zMin);
	//for (int x = xMin; x < xMax; ++x) {
	//	for (int z = zMin; z < zMax; ++z) {
	//		p.x = x;
	//		p.z = z;
	//		vertices.push_back(p);
	//		++p.x;
	//		vertices.push_back(p);
	//		--p.x;
	//		++p.z;
	//		vertices.push_back(p);
	//		vertices.push_back(p);
	//		++p.x;
	//		vertices.push_back(p);
	//		--p.z;
	//		vertices.push_back(p);
	//	}
	//}
	vec3 p1(0, 0, 0);
	vec3 p2(0, 0, 0);
	for (int x = xMin; x < xMax; ++x) {
		p1.x = x;
		p2.x = x;
		for (int z = zMin; z < zMax; ++z) {
			p1.z = z;
			p2.z = z + 1;
			vertices.push_back(p1);
			vertices.push_back(p2);
			vertices.push_back(p2);
		}
	}
	for (int z = zMin; z < zMax; ++z) {
		p1.z = z;
		p2.z = z;
		for (int x = xMin; x < xMax; ++x) {
			p1.x = x;
			p2.x = x + 1;
			vertices.push_back(p1);
			vertices.push_back(p2);
			vertices.push_back(p2);
		}
	}
}

PrimMeshModel::PrimMeshModel(const int s) : primitive_type(PYRAMID)
{
	vector<vec3> vt;
	GLfloat min_x = -1*s;
	GLfloat min_y = -1*s;
	GLfloat min_z = 0;
	GLfloat max_x = 1*s;
	GLfloat max_y = 1*s;
	GLfloat max_z = 1*s;
	vec3 bounds_min = vec3(min_x, min_y, min_z);
	vec3 bounds_max = vec3(max_x, max_y, max_z);
	this->bounding_box.push_back(bounds_min);
	this->bounding_box.push_back(bounds_max);
	vt.push_back(vec3(min_x, min_y, min_z)); //1
	vt.push_back(vec3(min_x, max_y, min_z)); //2
	vt.push_back(vec3(max_x, max_y, min_z)); //3
	vt.push_back(vec3(max_x, min_y, min_z)); //4
	vt.push_back(vec3((min_x + max_x) / 2, (min_y + max_y) / 2, max_z)); //5
	//f 1 4 5
	this->vertices.push_back(vec3(vt[0]));
	this->vertices.push_back(vec3(vt[3]));
	this->vertices.push_back(vec3(vt[4]));
	//f 4 3 5
	this->vertices.push_back(vec3(vt[3]));
	this->vertices.push_back(vec3(vt[2]));
	this->vertices.push_back(vec3(vt[4]));
	//f 3 2 5
	this->vertices.push_back(vec3(vt[2]));
	this->vertices.push_back(vec3(vt[1]));
	this->vertices.push_back(vec3(vt[4]));
	//f 2 1 5
	this->vertices.push_back(vec3(vt[1]));
	this->vertices.push_back(vec3(vt[0]));
	this->vertices.push_back(vec3(vt[4]));
	compute_face_normals();
}


PrimMeshModel::~PrimMeshModel()
{
}
void PrimMeshModel::draw(Renderer& renderer, COLORS color)
{
	if (primitive_type == XZ_GRID) {
		renderer.SetObjectMatrices(_world_transform, _normal_transform);
		renderer.DrawTriangles(&vertices, NULL, NULL, YELLOW);
	}
	if (primitive_type == PYRAMID) {
		renderer.SetObjectMatrices(_world_transform, _normal_transform);
		renderer.DrawTriangles(&vertices, NULL, NULL, color); //draw only triangles. default case.
		if (renderer.draw_triangle_normals)
			renderer.DrawFaceNormals(&face_normals);
		if (renderer.draw_bounding_box)
			renderer.DrawBoundingBox(&bounding_box);
	}
}

