#include "stdafx.h"
#include "Scene.h"
#include <string>
using namespace std;

inline void AddMesh(vector<vec3>& model, const vec3& p1, const vec3& p2, const vec3& p3)
{
	model.push_back(p1);
	model.push_back(p2);
	model.push_back(p3);
}

void CreateCameraModel(vector<vec3>& model, int length = 2)
{
	vec3 p1(0, 0, 0);
	vec3 p2(1, -1, -length);
	vec3 p3(1, 1, -length);
	vec3 p4(-1, 1, -length);
	vec3 p5(-1, -1, -length);
	vec3 p6(0, 2, -length);
	vec3 p7(0, 1, -length);
	vec3 p8(0.5, 1.5, -length);
	vec3 p9(-0.5, 1.5, -length);
	AddMesh(model, p1, p2, p3);
	AddMesh(model, p1, p3, p4);
	AddMesh(model, p1, p4, p5);
	AddMesh(model, p1, p5, p2);
	AddMesh(model, p6, p7, p7);
	AddMesh(model, p8, p9, p9);
}

Camera::Camera()
{
	CreateCameraModel(model);
	ChangeZoom(ZOOM_RESET);
	this->LookAt(vec3(rand() % 10, rand() % 10, rand() % 10), vec3(0, 0, 0) , vec3(0, 1, 0), true); //camera position XYZ= 0..9 range.
	this->Frustum(-1, 1, -0.5, 0.5, -1, -50); //default.
}

void Camera::setTransformation(const mat4& transform)
{
	view = transform;
}

void Camera::LookAt(const vec3& eye, const vec3& at, const vec3& up, bool change_angles)
{
	this->eye = eye;
	this->at = at;
	this->up = up;
	n = normalize(eye - at);
	u = normalize(cross(up, n));
	v = cross(n, u);
	vec4 t = vec4(0, 0, 0, 1);
	view = mat4(vec4(u, 0), vec4(v, 0), vec4(n, 0), t);
	cTransform = Translate(eye);
	cTransform.multiply(transpose(view));
	view.multiply(Translate(-eye));

	if (change_angles) {
		theta = acos(-n.y / sqrt(n.x * n.x + n.y * n.y + n.z * n.z));
		phi = (n.z != 0) ? atan(n.x / n.z) : M_PI / 4;
		if (n.z > 0) {
			phi += M_PI;
		}
		sinTheta = sin(theta);
		cosTheta = cos(theta);
		sinPhi = sin(phi);
		cosPhi = cos(phi);
	}
}

void Camera::Ortho(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar)
{
	projection = mat4();
	projection[0][0] = 2 / (right - left);
	projection[0][0] /= zoom;
	projection[1][1] = 2 / (top - bottom);
	projection[1][1] /= zoom;
	projection[2][2] = -2 / (zFar - zNear);
	projection[0][3] = -(right + left) / (right - left);
	projection[1][3] = -(top + bottom) / (top - bottom);
	projection[2][3] = (zFar + zNear) / (zFar - zNear);

	aspect_ratio = right - left;
	aspect_ratio /= (top - bottom);
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	this->zNear = zNear;
	this->zFar = zFar;
	this->projection_type = ORTHOGRAPHIC;
}

void Camera::Frustum(const float left, const float right,
	const float bottom, const float top,
	const float zNear, const float zFar)
{
	projection = mat4();
	projection[0][0] = -2 * zNear / (right - left);
	projection[0][0] /= zoom;
	projection[0][2] = (right + left) / (right - left);
	projection[1][1] = -2 * zNear / (top - bottom);
	projection[1][1] /= zoom;
	projection[1][2] = (top + bottom) / (top - bottom);
	projection[2][2] = (zFar + zNear) / (zFar - zNear);
	projection[2][3] = -2 * zFar * zNear / (zFar - zNear);
	projection[3][2] = -1;
	projection[3][3] = 0;

	aspect_ratio = right - left;
	aspect_ratio /= (top - bottom);
	this->left = left;
	this->right = right;
	this->bottom = bottom;
	this->top = top;
	this->zNear = zNear;
	this->zFar = zFar;
	this->projection_type = FRUSTUM;
}

// fovy in degrees
void Camera::Perspective(const float fovy, const float aspect,
	const float zNear, const float zFar)
{
	float tan_half_fovy = tan((M_PI * fovy / 180) / 2);
	projection = mat4();
	projection[0][0] = 1 / (aspect * tan_half_fovy);
	projection[0][0] /= zoom;
	projection[1][1] = 1 / tan_half_fovy;
	projection[1][1] /= zoom;
	projection[2][2] = (zFar + zNear) / (zFar - zNear);
	projection[2][3] = -2 * zFar * zNear / (zFar - zNear);
	projection[3][2] = -1;
	projection[3][3] = 0;

	aspect_ratio = aspect;
	this->fovy = fovy;
	this->aspect = aspect;
	this->zNear = zNear;
	this->zFar = zFar;
	this->projection_type = PERSPECTIVE;
}

mat4 Camera::GetProjection()
{
	return projection;
}

mat4 Camera::GetTransform()
{
	return view;
}

float Camera::GetAspectRatio()
{
	return aspect_ratio;
}

void Camera::Draw(Renderer& renderer, COLORS color)
{
	mat3 identity;
	renderer.SetObjectMatrices(cTransform, identity);
	renderer.DrawTriangles(&model, NULL, NULL, color, &material);
}

void Camera::UpdateProjection()
{
	switch (projection_type)
	{
	case ORTHOGRAPHIC:
		Ortho(left, right, bottom, top, zNear, zFar);
		break;
	case FRUSTUM:
		Frustum(left, right, bottom, top, zNear, zFar);
		break;
	case PERSPECTIVE:
		Perspective(fovy, aspect, zNear, zFar);
		break;
	}
}

void Camera::ChangeZoom(CAMERA_ZOOM zoom)
{
	switch (zoom) {
	case ZOOM_RESET:
		this->zoom = 1;
		UpdateProjection();
		break;
	case ZOOM_IN:
		this->zoom *= 0.9;
		UpdateProjection();
		break;
	case ZOOM_OUT:
		if (this->zoom < 1) {
			this->zoom *= 1.1;
			if (this->zoom > 1) {
				this->zoom = 1;
			}
			UpdateProjection();
		}
		break;
	}
}

void Camera::ChangeAngles(float dtheta, float dphi)
{
	theta += M_PI * dtheta / 180;
	phi += M_PI * dphi / 180;
	if (theta < 0) {
		theta = 0.00001;
	}
	else if (theta > M_PI) {
		theta = M_PI - 0.00001;
	}
	sinTheta = sin(theta);
	cosTheta = cos(theta);
	sinPhi = sin(phi);
	cosPhi = cos(phi);
	vec3 up_new(0, 1, 0);	
	vec3 at_new = this->eye + vec3(sinTheta * sinPhi, cosTheta, sinTheta * cosPhi);
	LookAt(this->eye, at_new, up_new, false);
}

void Camera::TranslateInFocusDirection(float distance)
{
	vec3 v(distance * sinTheta * sinPhi, distance * cosTheta, distance * sinTheta * cosPhi);
	this->eye += v;
	this->at += v;
	LookAt(this->eye, this->at, this->up, false);
}

void Camera::TranslateInRightDirection(float distance)
{
	vec3 v(-distance * cosPhi, 0, distance * sinPhi);
	this->eye += v;
	this->at += v;
	LookAt(this->eye, this->at, this->up, false);
}

void Camera::FocusOnPoint(const vec3& p)
{
	if (eye.x == p.x && eye.y == p.y && eye.z == p.z) {
		return;
	}
	if (eye.y == p.y) {
		LookAt(eye, vec3(p.x, p.y, p.z + 0.001), up, true);
	}
	else {
		LookAt(eye, p, up, true);
	}
}

Scene::Scene(Renderer *renderer) : m_renderer(renderer)
{
	Camera* c = new Camera();
	c->LookAt(vec3(5, 5, 5), vec3(0, 0, 0), vec3(0, 1, 0), true);
	//c->Frustum(-2, 2, -2, 2, -1, -50);

	LightSource lightSource;
	lightSource.sourceType = PARALLEL_LIGHT;
	lightSource.position = vec4(-1, -1, -1, 0);

	cameras.push_back(c);
	activeCamera = 0;
	activeModel = -1;
	xzGrid = new PrimMeshModel(-30, 30, -30, 30);
	draw_cameras = true;
}

void Scene::addCamera(Camera* camera)
{
	cameras.push_back(camera);
}

void Scene::loadOBJModel(string fileName)
{
	MeshModel *model = new MeshModel(fileName);
	models.push_back(model);
	activeModel = models.size() - 1;
}

void Scene::updateRendererLightSources()
{
	vector<LightSource> light_sources;
	for (int i = 0; i < lights.size(); ++i) {
		light_sources.push_back(lights[i]->light_source);
	}
	m_renderer->SetLightSources(&light_sources);
}

void Scene::draw(Renderer &renderer)
{
	// 1. Send the renderer the current camera transform and the projection
	Camera *camera = cameras[activeCamera];
	m_renderer->SetProjectionAndZLimits(camera->GetProjection(), camera->GetZFar() , camera->GetZNear());
	m_renderer->SetCameraTransform(camera->GetTransform());
	m_renderer->ClearColorBuffer();
	m_renderer->ClearDepthBuffer();
	// 2. Tell all models to draw themselves
	Model *model = activeModel > -1 ? models[activeModel] : NULL;
	Light *light = activeLight > -1 ? lights[activeLight] : NULL;
	if (draw_xzGrid) {
		this->xzGrid->draw(renderer, xzGridColor);
	}
	for each (Model* m in models)
	{
		if (m == model)
			m->draw(renderer, activeModelColor);
		else
			m->draw(renderer, modelColor);
	}
	if (draw_cameras) {
		for each (Camera* c in cameras)
		{
			if (c != camera)
				c->Draw(renderer, cameraColor);
		}
	}
	for each (Light* l in lights)
	{
		if (l == light)
			l->Draw(renderer);
		else
			l->Draw(renderer);

	}
	m_renderer->SwapBuffers();
}


void Scene::toggleXZGrid() {
	draw_xzGrid = !draw_xzGrid;
}

void Scene::showOrHideCameras() {
	draw_cameras = !draw_cameras;
}

void Scene::changeActiveModel() {
	if (models.size() > 0) {
		++activeModel;
		activeModel = activeModel % models.size();
	}
}

void Scene::changeActiveCamera() {
	++activeCamera;
	activeCamera = activeCamera % cameras.size();
}

void Scene::addModel(Model* model)
{
	models.push_back(model);
	activeModel = models.size() - 1;
}

float Scene::getActiveCameraAspectRatio()
{
	return cameras[activeCamera]->GetAspectRatio();
}

void Scene::changeCameraZoom(CAMERA_ZOOM zoom)
{
	Camera *camera = cameras[activeCamera];
	camera->ChangeZoom(zoom);
}

void Scene::changeCameraAngles(float dtheta, float dphi)
{
	Camera *camera = cameras[activeCamera];
	camera->ChangeAngles(dtheta, dphi);
}

void Scene::translateCameraInFocusDirection(float distance)
{
	Camera *camera = cameras[activeCamera];
	camera->TranslateInFocusDirection(distance);
}

void Scene::translateCameraInRightDirection(float distance)
{
	Camera *camera = cameras[activeCamera];
	camera->TranslateInRightDirection(distance);
}

void Scene::focusCameraOnActiveModel()
{
	if (activeModel == -1) {
		return;
	}
	Camera *camera = cameras[activeCamera];
	MeshModel* model = (MeshModel*)models[activeModel];
	vec4 p(0, 0, 0, 1);
	p = model->_world_transform * p;
	camera->FocusOnPoint(vec3(p.x, p.y, p.z));
}

void Scene::transformActiveModel(FRAMES frame, ACTIONS action, AXES axis, float amount)
{
	if (activeModel == -1 || (frame != MODEL && frame != WORLD)) {
		return;
	}
	MeshModel* model = (MeshModel*)models[activeModel];
	vec4 v(0, 0, 0, 1);
	switch (action) {
	case ROTATE:
		if (frame == MODEL) {
			v = model->_world_transform * v;
			model->_world_transform = Translate(-v) * model->_world_transform;
}
		rotateModel(model, axis, amount);
		if (frame == MODEL) {
			model->_world_transform = Translate(v) * model->_world_transform;
		}
		break;
	case TRANSLATE:
		if (frame == WORLD) {
			translateModel(model, axis, amount);
		}
		break;
	case SCALE:
		if (frame == MODEL) {
			v = model->_world_transform * v;
			model->_world_transform = Translate(-v) * model->_world_transform;
		}
		scaleModel(model, axis, amount);
		if (frame == MODEL) {
			model->_world_transform = Translate(v) * model->_world_transform;
		}
		break;
	}
}

void Scene::rotateModel(MeshModel *model, AXES axis, float theta)
{
	switch (axis) {
	case X_AXIS:
		model->_world_transform = RotateX(theta) * model->_world_transform;
		model->_normal_transform = transpose(RotateXmat3(-theta)) * model->_normal_transform;
		model->_normal_transform /= pow(model->_normal_transform.det(), 0.333333f);
		break;
	case Y_AXIS:
		model->_world_transform = RotateY(theta) * model->_world_transform;
		model->_normal_transform = transpose(RotateYmat3(-theta)) * model->_normal_transform;
		model->_normal_transform /= pow(model->_normal_transform.det(), 0.333333f);
		break;
	case Z_AXIS:
		model->_world_transform = RotateZ(theta) * model->_world_transform;
		model->_normal_transform = transpose(RotateZmat3(-theta)) * model->_normal_transform;
		model->_normal_transform /= pow(model->_normal_transform.det(), 0.333333f);
		break;
	}
}

void Scene::translateModel(MeshModel *model, AXES axis, float distance)
{
	switch (axis) {
	case X_AXIS:
		model->_world_transform = Translate(vec4(distance, 0, 0, 1)) * model->_world_transform;
		break;
	case Y_AXIS:
		model->_world_transform = Translate(vec4(0, distance, 0, 1)) * model->_world_transform;
		break;
	case Z_AXIS:
		model->_world_transform = Translate(vec4(0, 0, distance, 1)) * model->_world_transform;
		break;
	}
}

void Scene::scaleModel(MeshModel *model, AXES axis, float s)
{
	mat3 inverseScale;
	vec3 v(1, 1, 1);
	switch (axis) {
	case X_AXIS:
		v.x = s;
		inverseScale[0][0] = 1 / s;
		break;
	case Y_AXIS:
		v.y = s;
		inverseScale[1][1] = 1 / s;
		break;
	case Z_AXIS:
		v.z = s;
		inverseScale[2][2] = 1 / s;
		break;
	}
	model->_world_transform = Scale(v) * model->_world_transform;
	model->_normal_transform = inverseScale * model->_normal_transform;
	model->_normal_transform /= pow(model->_normal_transform.det(), 0.333333f);
}

void Scene::scaleActiveModel(vec3 s)
{
	if (activeModel == -1) {
		return;
	}
	MeshModel* model = (MeshModel*)models[activeModel];
	model->_world_transform = Scale(s) * model->_world_transform;
	mat3 inverseScale;
	inverseScale[0][0] = 1 / s.x;
	inverseScale[1][1] = 1 / s.y;
	inverseScale[2][2] = 1 / s.z;
	model->_normal_transform = inverseScale * model->_normal_transform;
	model->_normal_transform /= pow(model->_normal_transform.det(), 0.333333f);
}

const Material& Scene::getActiveModelMaterial()
{
	static const Material defaultMaterial = Material(vec3(0.33f, 0.22f, 0.03f), vec3(0.78f, 0.57f, 0.11f), vec3(0.99f, 0.91f, 0.81f), vec3(0.0f, 0.0f, 0.0f), 27.8f);
	if (activeModel == -1) {
		return defaultMaterial;
	}
	MeshModel* model = (MeshModel*)models[activeModel];
	return model->getModelMaterial().materials[0];
}

void Scene::setActiveModelMaterial(const Material& material)
{
	if (activeModel == -1) {
		return;
	}
	MeshModel* model = (MeshModel*)models[activeModel];
	ModelMaterial& activeModelMaterial = model->getModelMaterial();
	activeModelMaterial.materials[0] = material;
	activeModelMaterial.materials[1] = material;
	activeModelMaterial.materials[2] = material;
}

void Scene::toggleActiveModelUniformMaterial()
{
	if (activeModel == -1) {
		return;
	}
	MeshModel* model = (MeshModel*)models[activeModel];
	model->getModelMaterial().uniform = !(model->getModelMaterial().uniform);
}

Camera* Scene::getActiveCamera()
{
	return cameras.at(activeCamera);
}

void Scene::removeCamera()
{
	if (cameras.size()-1)
	{
		cameras.pop_back();
		this->changeActiveCamera();
	}
}

MeshModel* Scene::getActiveModel()
{
	if (models.size()) {
		return (MeshModel*)models.at(activeModel);
	}
	else {
		return NULL;
	}
}

void Scene::removeModel()
{
	if (models.size())
	{
		models.pop_back();
		if (models.size() == 0)
			this->activeModel = -1;
		else 
			this->changeActiveModel();
		
	}
}

void Scene::changeActiveLight()
{
	if (lights.size() > 0) {
		++activeLight;
		activeLight = activeLight % lights.size();
	}
}

void Scene::removeLight()
{
	if (lights.size())
	{
		lights.pop_back();
		updateRendererLightSources();
		if (lights.size() == 0)
			this->activeLight = -1;
		else
			this->changeActiveLight();
	}
}
Light* Scene::getActiveLight()
{
	return lights.at(activeLight);
}

void Scene::transformActiveLight(FRAMES frame, ACTIONS action, AXES axis, float amount)
{
	if (activeLight == -1) {
		return;
	}
	Light* light = lights[activeLight];
	switch (action) {
	case ROTATE:
		light->RotateLight(axis, amount);
		break;
	case TRANSLATE:
		light->TranslateLight(axis, amount);
		break;
	case SCALE:
		light->ScaleLight(amount);
		break;
	}
	updateRendererLightSources();
}

void Scene::changeActiveLightComponent(const RGB colorComponent, const LIGHT_COMPONENT lightComponent, const float amount) {
	if (activeLight == -1) {
		return;
	}
	Light* light = lights[activeLight];
	light->ChangeComponent(colorComponent, lightComponent, amount);
	updateRendererLightSources();
}

void Scene::resetActiveLightToWhite() {
	if (activeLight == -1) {
		return;
	}
	Light* light = lights[activeLight];
	light->ResetToWhiteLight();
	updateRendererLightSources();
}

const LightSource& Scene::getActiveLightSource()
{
	static const LightSource defaultLightSource;
	if (activeLight == -1) {
		return defaultLightSource;
	}
	Light* light = lights[activeLight];
	return light->light_source;
}

void Scene::setActiveLightSource(const LightSource& lightSource)
{
	if (activeLight == -1) {
		return;
	}
	Light* light = lights[activeLight];
	light->light_source = lightSource;
	updateRendererLightSources();
}

void Scene::addLightSource(Light* light)
{
	lights.push_back(light);
	activeLight = lights.size()-1;
	updateRendererLightSources();
}

vec3 v[4] = {
	vec3(0.0, 0.0, 1.0),
	vec3(0.0, 0.942809, -0.333333),
	vec3(-0.816497, -0.471405, -0.333333),
	vec3(0.816497, -0.471405, -0.333333)
};

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
		AddMesh(model, a, b, c);
	}
}

void CreateTetrahedron(vector<vec3>& model, int n) {
	DivideTriangle(model, v[0], v[1], v[2], n);
	DivideTriangle(model, v[3], v[2], v[1], n);
	DivideTriangle(model, v[0], v[3], v[1], n);
	DivideTriangle(model, v[0], v[2], v[3], n);
}

void CreateArrowToPositiveY(vector<vec3>& model, int height, float scale = 0.5) {
	if (height <= 3) {
		throw exception("The arrow height must be bigger than 3");
	}
	vec3 p1(0.5, 0, 0.5);
	vec3 p2(0.5, 0, -0.5);
	vec3 p3(-0.5, 0, -0.5);
	vec3 p4(-0.5, 0, 0.5);
	vec3 p5(0.5, height-2, 0.5);
	vec3 p6(0.5, height-2, -0.5);
	vec3 p7(-0.5, height-2, -0.5);
	vec3 p8(-0.5, height-2, 0.5);
	vec3 p9(1, height-2, 1);
	vec3 p10(1, height-2, -1);
	vec3 p11(-1, height-2, -1);
	vec3 p12(-1, height-2, 1);
	vec3 p13(0, height, 0);

	p1 *= scale;
	p2 *= scale;
	p3 *= scale;
	p4 *= scale;
	p5 *= scale;
	p6 *= scale;
	p7 *= scale;
	p8 *= scale;
	p9 *= scale;
	p10 *= scale;
	p11 *= scale;
	p12 *= scale;
	p13 *= scale;

	AddMesh(model, p1, p3, p2);
	AddMesh(model, p1, p4, p3);
	AddMesh(model, p1, p2, p5);
	AddMesh(model, p2, p6, p5);
	AddMesh(model, p4, p1, p8);
	AddMesh(model, p1, p5, p8);
	AddMesh(model, p3, p4, p7);
	AddMesh(model, p4, p8, p7);
	AddMesh(model, p2, p3, p6);
	AddMesh(model, p3, p7, p6);
	AddMesh(model, p5, p9, p12);
	AddMesh(model, p5, p12, p8);
	AddMesh(model, p11, p8, p12);
	AddMesh(model, p11, p7, p8);
	AddMesh(model, p11, p10, p7);
	AddMesh(model, p7, p10, p6);
	AddMesh(model, p10, p9, p6);
	AddMesh(model, p5, p6, p9);
	AddMesh(model, p13, p12, p9);
	AddMesh(model, p13, p11, p12);
	AddMesh(model, p13, p10, p11);
	AddMesh(model, p13, p9, p10);
}

Light::Light() {
	CreateTetrahedron(modelPointLight, 3);
	CreateArrowToPositiveY(modelParallelLight, 6);
	ChangeToParallelSource();
	ScaleLight(0.5);
	AdjustMaterial();
}

void Light::AdjustMaterial() {
	static const vec3 blackColor(0.0, 0.0, 0.0);
	vec3 totalLight = light_source.ambient;
	totalLight += light_source.diffuse;
	totalLight += light_source.specular;
	float sum = totalLight.x + totalLight.y + totalLight.z;
	float max = totalLight.x;
	if (totalLight.y > max) {
		max = totalLight.y;
	}
	if (totalLight.z > max) {
		max = totalLight.z;
	}
	totalLight /= max;
	material.materials[0].ambient = blackColor;
	material.materials[0].diffuse = blackColor;
	material.materials[0].specular = blackColor;
	material.materials[0].emission = totalLight;
}

void Light::UpdateLightSourcePosition() {
	static const vec4 positiveY(0, 1, 0, 1);
	static const vec4 origin(0, 0, 0, 1);
	if (light_source.sourceType == POINT_LIGHT) {
		light_source.position = translation * origin;
	}
	else if (light_source.sourceType == PARALLEL_LIGHT) {
		light_source.position = rotation * positiveY;
	}
}

void Light::ChangeToPointSource() {
	light_source.sourceType = POINT_LIGHT;
	UpdateLightSourcePosition();
}

void Light::ChangeToParallelSource() {
	light_source.sourceType = PARALLEL_LIGHT;
	UpdateLightSourcePosition();
}

void Light::RotateLight(AXES axis, const float theta) {
	switch (axis) {
	case X_AXIS:
		rotation = RotateX(theta) * rotation;
		break;
	case Y_AXIS:
		rotation = RotateY(theta) * rotation;
		break;
	case Z_AXIS:
		rotation = RotateZ(theta) * rotation;
		break;
	}
	world_transform = translation;
	world_transform.multiply(rotation);
	world_transform.multiply(scale);
	UpdateLightSourcePosition();
}

void Light::TranslateLight(AXES axis, const float distance) {
	switch (axis) {
	case X_AXIS:
		translation = Translate(vec4(distance, 0, 0, 1)) * translation;
		break;
	case Y_AXIS:
		translation = Translate(vec4(0, distance, 0, 1)) * translation;
		break;
	case Z_AXIS:
		translation = Translate(vec4(0, 0, distance, 1)) * translation;
		break;
	}
	world_transform = translation;
	world_transform.multiply(rotation);
	world_transform.multiply(scale);
	UpdateLightSourcePosition();
}

void Light::ScaleLight(const float s) {
	scale.multiply(Scale(s, s, s));
	world_transform = translation;
	world_transform.multiply(rotation);
	world_transform.multiply(scale);
	UpdateLightSourcePosition();
}

void Light::ChangeComponent(const RGB colorComponent, const LIGHT_COMPONENT lightComponent, const float amount) {
	vec3 colorChange(1.0, 1.0, 1.0);
	switch (colorComponent) {
	case R_RGB:
		colorChange.x = amount;
		break;
	case G_RGB:
		colorChange.y = amount;
		break;
	case B_RGB:
		colorChange.z = amount;
		break;
	case ALL_RGB:
		colorChange.x = amount;
		colorChange.y = amount;
		colorChange.z = amount;
		break;
	}
	switch (lightComponent) {
	case LIGHT_AMBIENT:
		light_source.ambient *= colorChange;
		break;
	case LIGHT_DIFFUSE:
		light_source.diffuse *= colorChange;
		break;
	case LIGHT_SPECULAR:
		light_source.specular *= colorChange;
		break;
	case LIGHT_ALL_COMPONENTS:
		light_source.ambient *= colorChange;
		light_source.diffuse *= colorChange;
		light_source.specular *= colorChange;
		break;
	}
	AdjustMaterial();
}

void Light::ResetToWhiteLight() {
	static vec3 whiteLight(3.0, 3.0, 3.0);
	//light_source.ambient = whiteLight;
	light_source.diffuse = whiteLight;
	light_source.specular = whiteLight;
	AdjustMaterial();
}

void Light::Draw(Renderer& renderer) {
	static const mat3 normal_transform(1.0);
	renderer.SetObjectMatrices(world_transform, normal_transform);
	switch (light_source.sourceType) {
	case POINT_LIGHT:
		renderer.DrawTriangles(&modelPointLight, NULL, NULL, CYAN, &material);
		break;
	case PARALLEL_LIGHT:
		renderer.DrawTriangles(&modelParallelLight, NULL, NULL, CYAN, &material);
		break;
	}
}