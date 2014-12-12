#pragma once
#include "Renderer.h"
#include "mat.h"

struct LightSource
{
	vec4 ambient;              // Aclarri
	vec4 diffuse;              // Dcli
	vec4 specular;             // Scli
	vec4 position;             // Ppli
	vec4 halfVector;           // Derived: Hi
	vec3 spotDirection;        // Sdli
	float spotExponent;        // Srli
	float spotCutoff;          // Crli
	// (range: [0.0,90.0], 180.0)
	float spotCosCutoff;       // Derived: cos(Crli)
	// (range: [1.0,0.0],-1.0)
	float constantAttenuation; // K0
	float linearAttenuation;   // K1
	float quadraticAttenuation;// K2

	vector<vec3> model;
	LIGHT_SOURCE_TYPE source_type;

	LightSource(LIGHT_SOURCE_TYPE source_type = POINT_LIGHT);
	vec4 GetPoisition();
	void Draw(Renderer& renderer);
	~LightSource();
};