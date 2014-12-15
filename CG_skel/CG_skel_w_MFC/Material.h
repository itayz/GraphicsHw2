#pragma once
#include "vec.h"
#include <string>
using namespace std;

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emission;
	GLfloat shininess;
	Material() {};
	Material(const vec3& a, const vec3& d, const vec3& s, const vec3& e, GLfloat shi) : ambient(a), diffuse(d), specular(s), emission(e), shininess(shi) {};
	~Material() {};
};

class Materials
{
public:
	/*enum materials {
	emerald, jade, obsidian, pearl, ruby, turquoise, brass, bronze,
	chrome, copper, gold, silver, black_plastic, cyan_plastic, green_plastic
	, red_plastic, white_plastic, yellow_plasti, black_rubber, cyan_rubber
	, green_rubber, red_rubber, white_rubber, yellow_rubber
	};*/
	/*Material	emerald = { { 0.0215f, 0.1745f, 0.0215f }, { 0.07568f, 0.61424f, 0.07568f }, { 0.633f, 0.727811f, 0.633f }, 0.6f };
	Material	jade = { { 0.135f, 0.2225f, 0.1575f }, { 0.54f, 0.89f, 0.63f }, { 0.316228f, 0.316228f, 0.316228f }, 0.1f };
	Material	obsidian = { { 0.05375f, 0.05f, 0.06625f }, { 0.18275f, 0.17f, 0.22525f }, { 0.332741f, 0.328634f, 0.346435f }, 0.3f };
	Material	pearl = { { 0.25f, 0.20725f, 0.20725f }, { 1.0f, 0.829f, 0.829f }, { 0.296648f, 0.296648f, 0.296648f }, 0.088f };
	Material	ruby = { { 0.1745f, 0.01175f, 0.01175f }, { 0.61424f, 0.04136f, 0.04136f }, { 0.727811f, 0.626959f, 0.626959f }, 0.6f };
	Material	turquoise = { { 0.1f, 0.18725f, 0.1745f }, { 0.396f, 0.74151f, 0.69102f }, { 0.297254f, 0.30829f, 0.306678f }, 0.1f };
	Material	brass = { { 0.329412f, 0.223529f, 0.027451f }, { 0.780392f, 0.568627f, 0.113725f }, { 0.992157f, 0.941176f, 0.807843f }, 0.21794872f };
	Material	bronze = { { 0.2125f, 0.1275f, 0.054f }, { 0.714f, 0.4284f, 0.18144f }, { 0.393548f, 0.271906f, 0.166721f }, 0.2f };
	Material	chrome = { { 0.25f, 0.25f, 0.25f }, { 0.4f, 0.4f, 0.4f }, { 0.774597f, 0.774597f, 0.774597f }, 0.6f };
	Material	copper = { { 0.19125f, 0.0735f, 0.0225f }, { 0.7038f, 0.27048f, 0.0828f }, { 0.256777f, 0.137622f, 0.086014f }, 0.1f };
	Material	gold = { { 0.24725f, 0.1995f, 0.0745f }, { 0.75164f, 0.60648f, 0.22648f }, { 0.628281f, 0.555802f, 0.366065f }, 0.4f };
	Material	silver = { { 0.19225f, 0.19225f, 0.19225f }, { 0.50754f, 0.50754f, 0.50754f }, { 0.508273f, 0.508273f, 0.508273f }, 0.4f };
	Material	black_plastic = { { 0.0f, 0.0f, 0.0f }, { 0.01f, 0.01f, 0.01f }, { 0.50f, 0.50f, 0.50f }, .25f };
	Material	cyan_plastic = { { 0.0f, 0.1f, 0.06f }, { 0.0f, 0.50980392f, 0.50980392f }, { 0.50196078f, 0.50196078f, 0.50196078f }, .25f };
	Material	green_plastic = { { 0.0f, 0.0f, 0.0f }, { 0.1f, 0.35f, 0.1f }, { 0.45f, 0.55f, 0.45f }, .25f };
	Material	red_plastic = { { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.0f, 0.0f }, { 0.7f, 0.6f, 0.6f }, .25f };
	Material	white_plastic = { { 0.0f, 0.0f, 0.0f }, { 0.55f, 0.55f, 0.55f }, { 0.70f, 0.70f, 0.70f }, .25f };
	Material	yellow_plastic = { { 0.0f, 0.0f, 0.0f }, { 0.5f, 0.5f, 0.0f }, { 0.60f, 0.60f, 0.50f }, .25f };
	Material	black_rubber = { { 0.02f, 0.02f, 0.02f }, { 0.01f, 0.01f, 0.01f }, { 0.4f, 0.4f, 0.4f }, .078125f };
	Material	cyan_rubber = { { 0.0f, 0.05f, 0.05f }, { 0.4f, 0.5f, 0.5f }, { 0.04f, 0.7f, 0.7f }, .078125f };
	Material	green_rubber = { { 0.0f, 0.05f, 0.0f }, { 0.4f, 0.5f, 0.4f }, { 0.04f, 0.7f, 0.04f }, .078125f };
	Material	red_rubber = { { 0.05f, 0.0f, 0.0f }, { 0.5f, 0.4f, 0.4f }, { 0.7f, 0.04f, 0.04f }, .078125f };
	Material	white_rubber = { { 0.05f, 0.05f, 0.05f }, { 0.5f, 0.5f, 0.5f }, { 0.7f, 0.7f, 0.7f }, .078125f };
	Material	yellow_rubber = { { 0.05f, 0.05f, 0.0f }, { 0.5f, 0.5f, 0.4f }, { 0.7f, 0.7f, 0.04f }, .078125f };

	Material material_name = silver; //current material */
};

struct ModelMaterial {
	bool uniform;
	Material materials[3];

	ModelMaterial() : uniform(true) {
		Material brass = Material(vec3(0.33f, 0.22f, 0.03f), vec3(0.78f, 0.57f, 0.11f), vec3(0.99f, 0.91f, 0.81f), vec3(0.0f, 0.0f, 0.0f), 27.8f);
		materials[0] = brass;
		materials[1] = brass;
		materials[2] = brass;
	}

	ModelMaterial(const Material& m) : uniform(true) {
		materials[0] = m;
		materials[1] = m;
		materials[2] = m;
	}

	ModelMaterial(const Material& m1, const Material& m2, const Material& m3) : uniform(false) {
		materials[0] = m1;
		materials[1] = m2;
		materials[2] = m3;
	}
};



