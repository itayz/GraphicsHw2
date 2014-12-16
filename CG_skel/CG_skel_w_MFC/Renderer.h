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
		//delete xLimits;
	}

	void ResetLimits() {
		for (int i = 2 * yMin; i <= 2 * yMax; i += 2) {
			xLimits[i] = INT_MAX;
			xLimits[i + 1] = INT_MIN;
		}
	}

	void SetLimits(int x, int y) {
		int i = 2 * y;
		int& xMin = xLimits[i];
		if (x < xMin) {
			xMin = x;
		}
		int& xMax = xLimits[i + 1];
		if (x > xMax) {
			xMax = x;
		}
	}
};

class Renderer
{
	float *m_outBuffer; // 3*width*height
	float *m_zbuffer; // width*height
	float *m_aa_outBuffer; //(3*width*height)*4
	float *m_aa_zbuffer; //(width*height)*4
	int m_width, m_height,m_aa_width,m_aa_height;
	float camera_aspect_ratio;
	mat4 viewTransform;
	mat4 projection;
	mat4 ndcToScreen,aa_ndcToScreen;
	mat4 oTransform;
	mat3 nTransform;
	vector<LightSource> lightSources;
	Fog* fog;
	SHADING_TYPES shadingType = FLAT_SHADING;
	vec3 eye;

	const float	normal_length = 0.3f; //normal scale for presentation.
	void CreateBuffers(int width, int height);
	void CreateLocalBuffer();
	void PlotPixel(int x, int y, COLORS color = CYAN);
	bool IsInsideScreen(int x, int y);

	float GetPixelZValue(int x, int y);
	void DrawPixel(int x, int y, float z, const vec4& color);
	void CalculateScanLines(int x1, int y1, int x2, int y2, int x3, int y3, ScanLines& scanLines);
	void BresenhamAlgorithm(int x1, int y1, int x2, int y2, ScanLines& scanLines);
	void ShadingColor(const vec3& p, const vec3& eye, const vec3& n, const Material& material, vec4& color);

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
	bool antialiasing_mode = false;
	///////////////////////////////////

	Renderer();
	Renderer(int width, int height);
	~Renderer(void);
	void Init();
	void DrawTriangles(const vector<vec3>* vertices, const vector<vec3>* v_normals = NULL, const vector<vec3>* f_normals = NULL, COLORS color = CYAN, const ModelMaterial* material = NULL);
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
	GLfloat sumQuadPixels(int x, int y, const int& color);
	void antiAlias();
	void SetShadingType(SHADING_TYPES shading);
	void SetEye(const vec3& eye);
};
