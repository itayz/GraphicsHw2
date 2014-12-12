#include "stdafx.h"
#include "LightSource.h"


LightSource::LightSource(COLORS c, LIGHT_SOURCE_TYPE s_l) :color(c), source_light(s_l) 
{
	this->loadFile("bulb.obj");
};


void LightSource::draw(Renderer& renderer, COLORS color)
{
	renderer.SetObjectMatrices(_world_transform, _normal_transform);
	renderer.DrawTriangles(&vertices, NULL, NULL, this->color); //draw only triangles. default case.
	if (renderer.draw_triangle_normals)
		return;
	if (renderer.draw_vertex_normals)
		return;
	if (renderer.draw_bounding_box)
		renderer.DrawBoundingBox(&bounding_box);
}

LightSource::~LightSource()
{
}

vec4 LightSource::getPoisition()
{
	return transpose(_world_transform)[3]; 
}