#pragma once

#include "gl/glew.h"
#include <vector>
#include <string>
#include "Renderer.h"
#include "MeshModel.h"
#include "PrimMeshModel.h"
#include "LightSource.h"
using namespace std;

enum CAMERA_ZOOM {
	ZOOM_RESET, ZOOM_IN, ZOOM_OUT
};

class Light {
	vector<vec3> modelPointLight;
	vector<vec3> modelParallelLight;
	ModelMaterial material;


public:
	LightSource light_source;

	Light();
	vec4 GetPoisition();
	void Draw(Renderer& renderer);
	~Light();
};

class Camera {
	enum PROJECTIONS {
		ORTHOGRAPHIC, FRUSTUM, PERSPECTIVE
	};

	mat4 cTransform;
	mat4 projection;
	mat4 view;
	float aspect_ratio;

	vector<vec3> model;

	float left, right, bottom, top, zNear, zFar, fovy, aspect, zoom;
	float theta, phi, sinTheta, cosTheta, sinPhi, cosPhi;
	PROJECTIONS projection_type;

	void UpdateProjection();

public:
	vec3 n, u, v;
	vec3 eye, at, up;
	Camera();
	void setTransformation(const mat4& transform);
	void LookAt(const vec3& eye, const vec3& at, const vec3& up, bool change_angles);
	void Ortho(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar);
	void Frustum(const float left, const float right,
		const float bottom, const float top,
		const float zNear, const float zFar);
	void Perspective(const float fovy, const float aspect, //fovy in degrees.
		const float zNear, const float zFar);
	mat4 GetProjection();
	mat4 GetTransform();
	float GetAspectRatio();
	void Draw(Renderer& renderer, COLORS color = CYAN);
	void ChangeZoom(CAMERA_ZOOM zoom);
	void ChangeAngles(float dtheta, float dphi);
	void TranslateInFocusDirection(float distance);
	void TranslateInRightDirection(float distance);
	void FocusOnPoint(const vec3& p);
	float GetZNear(){ return zNear; };
	float GetZFar(){ return zFar; };
};

class Scene {

	vector<Model*> models;
	vector<Light*> lights;
	vector<Camera*> cameras;
	Renderer *m_renderer;
	PrimMeshModel *xzGrid;
	bool draw_xzGrid = false;
	bool draw_cameras = false;

	const COLORS cameraColor = BLUE;
	const COLORS lightBulbColor = WHITE;
	const COLORS modelColor = CYAN;
	const COLORS activeModelColor = GREEN;
	const COLORS xzGridColor = RED;

	

	void rotateModel(MeshModel *model, AXES axis, float theta);
	void translateModel(MeshModel *model, AXES axis, float distance);
	void scaleModel(MeshModel *model, AXES axis, float s);

public:
	Scene() {};
	Scene(Renderer *renderer);
	void loadOBJModel(string fileName);
	void draw(Renderer &renderer);
	void toggleXZGrid();
	void showOrHideCameras();
	void changeActiveModel();
	void changeActiveCamera();
	//Light funcs
	void changeActiveLight();
	void addLightSource(Light* light);
	void removeLight();
	Light* getActiveLight();
	void transformActiveLight(FRAMES frame, ACTIONS action, AXES axis, float amount);
	void updateRendererLightSources();
	//
	void addModel(Model* model);
	float getActiveCameraAspectRatio();
	void changeCameraZoom(CAMERA_ZOOM zoom);
	void changeCameraAngles(float dtheta, float dphi);
	void translateCameraInFocusDirection(float distance);
	void translateCameraInRightDirection(float distance);
	void focusCameraOnActiveModel();
	void transformActiveModel(FRAMES frame, ACTIONS action, AXES axis, float amount);
	void scaleActiveModel(vec3 p);
	void addCamera(Camera* camera); //adds camera
	MeshModel* getActiveModel();
	void removeCamera();
	void removeModel();
	Camera* getActiveCamera();
	int activeModel;
	int activeLight;
	int activeCamera;

};