#pragma once
#include "vec.h"
class Fog
{
private:
	//linear implementation.
	vec4 fog_color;
	float* start; //view volume start end limits.
	float* end;

public:
	Fog(float* _start, float* _end,const vec3& c) :start(_start), end(_end),fog_color(c){}; 
	vec4 draw(const GLfloat& depth,const vec4& pixel_color); //return averaged color pixel.
	~Fog();
};

