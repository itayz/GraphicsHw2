#include "stdafx.h"
#include "Fog.h"


vec4 Fog::draw(const GLfloat& depth, const vec4& pixel_color) 
{
	GLfloat t= (*end - depth) / (*end - *start);
	return ((t*pixel_color) + (1 - t)*this->fog_color);
};