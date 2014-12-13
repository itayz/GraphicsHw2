#pragma once
#include "renderer.h"
#include "vec.h"
#include "mat.h"
#include <string>
#include "Material.h"

using namespace std;

class Model {
protected:
	virtual ~Model() {}
public:
	void virtual draw(Renderer& renderer, COLORS color = CYAN) = 0;
};

class MeshModel : public Model
{
protected :
	MeshModel() {}
	vector<vec3> vertices;
	//add more attributes
	vec3				bounds_min; //bounding box of our model.
	vec3				bounds_max; //bounding box of our model.
	vector<vec2>		vertex_texture; //OBJ vt
	vector<vec3>		vertex_normals; //OBJ vn
	vector<vec3>		face_normals; //[tail,normal,...] tail= position of the normal on the triangle. normal-tail= real normal
	vector<vec3>		bounding_box; //[min, max]
	ModelMaterial material;

public:
	mat4 _world_transform;
	mat3 _normal_transform;
	MeshModel(string fileName, bool uniform = true);
	~MeshModel(void);
	void loadFile(string fileName);
	void draw(Renderer& renderer, COLORS color = CYAN);
	void compute_face_normals();
	void setModelMaterial(const ModelMaterial& m);
	ModelMaterial& getModelMaterial() { return material; };
};
