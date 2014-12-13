#pragma once
#include <vector>
#include "CG_skel_w_MFC.h"
#include "vec.h"
#include "mat.h"
#include "GL/glew.h"
#include "LightSource.h"
#include "Material.h"
#include "Fog.h"

using namespace std;

struct ScanLines
{
	int yMin, yMax;
	int *xLimits;

	ScanLines(int screenHeight) {
		xLimits = new int[screenHeight * 2];
	}

	~ScanLines() {
		delete xLimits;
	}

	void ResetLimits() {
		for (int i = 2 * yMin; i <= 2 * yMax; i += 2) {
			xLimits[i] = INT_MAX;
			xLimits[i + 1] = INT_MIN;
		}
	}

	inline void SetLimits(int x, int y) {
		int i = 2 * y;
		if (x < xLimits[i]) {
			xLimits[i] = x;
		}
		if (x > xLimits[i + 1]) {
			xLimits[i + 1] = x;
		}
	}
};

class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	int m_width, m_height;
	float camera_aspect_ratio;
	mat4 viewTransform;
	mat4 projection;
	mat4 ndcToScreen;
	mat4 oTransform;
	mat3 nTransform;
	vector<LightSource> lightSources;
	Fog* fog;

	const float	normal_length = 0.3f; //normal scale for presentation.
	void CreateBuffers(int width, int height);
	void CreateLocalBuffer();
	void PlotPixel(int x, int y, COLORS color = CYAN);
	bool IsInsideScreen(int x, int y);

	void DrawPixel(int x, int y, float z, const vec4& color);
	void CalculateScanLines(int x1, int y1, int x2, int y2, int x3, int y3, ScanLines& scanLines);
	void BresenhamAlgorithm(int x1, int y1, int x2, int y2, ScanLines& scanLines);
	vec4 FlatShadingColor(const vec3& p, const vec3& eye, const vec3& n, const Material& material);

	//////////////////////////////
	// openGL stuff. Don't touch.

	GLuint gScreenTex;
	GLuint gScreenVtc;
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();
	//////////////////////////////
public:
	/////////////////////////////////// 
	//will be set in the UI.
	bool draw_triangle_normals = false;
	bool draw_vertex_normals = false;
	bool draw_bounding_box = false;
	bool draw_fog = false;
	///////////////////////////////////

	Renderer();
	Renderer(int width, int height);
	~Renderer(void);
	void Init();
	void DrawTriangles(const vector<vec3>* vertices, const vector<vec3>* v_normals = NULL, const vector<vec3>* f_normals = NULL, COLORS color = CYAN, const ModelMaterial* material = NULL, SHADING_TYPES shading_type = FLAT_SHADING);
	void DrawBoundingBox(const vector<vec3>* bounding_box, COLORS color = CYAN);
	void DrawFaceNormals(const vector<vec3>* f_normals);
	void DrawVertexNormals(const vector<vec3>* vertices,const vector<vec3>* v_normals);
	void SetCameraTransform(const mat4& cTransform);
	void SetProjection(const mat4& projection);
	void SetObjectMatrices(const mat4& oTransform, const mat3& nTransform);
	void AdjustToCameraAspectRatio(float camera_aspect_ratio);
	void SetLightSources(const vector<LightSource>* light_sources);
	void UpdateScreenSize(int width, int height);
	void SwapBuffers();
	void ClearColorBuffer();
	void ClearDepthBuffer();
	void setFog(Fog* f){ fog = f; };
};
