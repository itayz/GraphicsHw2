#include "stdafx.h"
#include "Fog.h"


vec3 Fog::operator()(const GLfloat& depth, const vec3& pixel_color) 
{
	GLfloat t= (end - depth) / (end - start);
	return ((t*pixel_color) + (1 - t)*this->fog_color);
};