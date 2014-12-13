#pragma once
#include "mat.h"

enum LIGHT_SOURCE_TYPE {
	POINT_LIGHT, PARALLEL_LIGHT
};

struct LightSource
{
	LIGHT_SOURCE_TYPE sourceType;
	vec3 ambient;              // Aclarri
	vec3 diffuse;              // Dcli
	vec3 specular;             // Scli
	vec4 position;             // Ppli
	vec4 halfVector;           // Derived: Hi
	vec3 spotDirection;        // Sdli
	float spotExponent;        // Srli
	float spotCutoff;          // Crli
	// (range: [0.0,90.0], 180.0)
	float spotCosCutoff;       // Derived: cos(Crli)
	// (range: [1.0,0.0],-1.0)
	float constantAttenuation = 1.0; // K0
	float linearAttenuation = 1.0;   // K1
	float quadraticAttenuation = 1.0;// K2

	LightSource() {
		sourceType = PARALLEL_LIGHT;
		ambient = vec3(0.8f, 0.8f, 0.8f);
		diffuse = vec3(0.8f, 0.8f, 0.8f);
		specular = vec3(0.8f, 0.8f, 0.8f);
		position = vec4(-1, -1, -1, 0);
	}
};