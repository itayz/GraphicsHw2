#pragma once
#include "vec.h"
class Fog
{
private:
	//linear implementation.
	vec3 fog_color;
	GLfloat start, end; //view volume start end limits.


public:
	Fog(const GLfloat& _start, const GLfloat& _end,const vec3& c) :start(_start), end(_end),fog_color(c){}; 
	vec3 operator()(const GLfloat& depth,const vec3& pixel_color);
	~Fog();
};

