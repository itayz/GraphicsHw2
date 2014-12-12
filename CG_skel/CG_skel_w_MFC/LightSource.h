#pragma once
#include "mat.h"

struct LightSource
{
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
	float constantAttenuation; // K0
	float linearAttenuation;   // K1
	float quadraticAttenuation;// K2
};