#pragma once
#include "MeshModel.h"
#include "Renderer.h"
#include "mat.h"

class LightSource : public MeshModel
{
private:
	//vec4 position; //fourth vector of world matrix.
	COLORS color;
	LIGHT_SOURCE_TYPE source_light;



public:
	LightSource(COLORS c, LIGHT_SOURCE_TYPE s_l = POINT_LIGHT);
	void setSourceLightType(LIGHT_SOURCE_TYPE l){ source_light = l; };
	vec4 getPoisition();
	COLORS getColor(){ return color; };
	void setColor(COLORS c){ color = c; };
	void draw(Renderer& renderer, COLORS color = YELLOW);
	~LightSource();
};