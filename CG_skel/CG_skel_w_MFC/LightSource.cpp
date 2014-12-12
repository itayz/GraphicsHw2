#include "stdafx.h"
#include "LightSource.h"

/*vec3 v[4] = {
	vec3(0, 0, 1),
	vec3(0, 2 * sqrt(2) / 3, -1 / 3),
	vec3(-sqrt(6) / 3, -sqrt(2) / 3, -1 / 3),
	vec3(sqrt(6) / 3, -sqrt(2) / 3, -1 / 3)
};*/

vec3 v[4] = {
	vec3(0.0, 0.0, 1.0),
	vec3(0.0, 0.942809, -0.333333),
	vec3(-0.816497, -0.471405, -0.333333),
	vec3(0.816497, -0.471405, -0.333333)
};

inline void Triangle(vector<vec3>& model, const vec3& p1, const vec3& p2, const vec3& p3) {
	model.push_back(p1);
	model.push_back(p2);
	model.push_back(p3);
}

void DivideTriangle(vector<vec3>& model, const vec3& a, const vec3& b, const vec3& c, int n) {
	vec3 v1, v2, v3;
	if (n > 0) {
		v1 = normalize(a + b);
		v2 = normalize(a + c);
		v3 = normalize(b + c);
		DivideTriangle(model, a, v2, v1, n - 1);
		DivideTriangle(model, c, v3, v2, n - 1);
		DivideTriangle(model, b, v1, v3, n - 1);
		DivideTriangle(model, v1, v2, v3, n - 1);
	}
	else {
		Triangle(model, a, b, c);
	}
}

void Tetrahedron(vector<vec3>& model, int n) {
	DivideTriangle(model, v[0], v[1], v[2], n);
	DivideTriangle(model, v[3], v[2], v[1], n);
	DivideTriangle(model, v[0], v[3], v[1], n);
	DivideTriangle(model, v[0], v[2], v[3], n);
}

LightSource::LightSource(LIGHT_SOURCE_TYPE source_type) {
	Tetrahedron(model, 3);
}

void LightSource::Draw(Renderer& renderer) {
	renderer.DrawTriangles(&model);
}