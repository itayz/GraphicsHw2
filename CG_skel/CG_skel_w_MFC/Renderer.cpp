#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"
#include "Triangle2D.h"

#define INDEX(width,x,y,c) (x+y*width)*3+c

mat4 CreateNdcToScreenMatrix(int width, int height, float camera_aspect_ratio)
{
	float screen_aspect_ratio = width;
	float heightScale, widthScale;
	screen_aspect_ratio /= height;
	mat4 ndcToScreen;
	if (screen_aspect_ratio > camera_aspect_ratio) {
		heightScale = height - 1;
		heightScale /= 2;
		widthScale = heightScale * camera_aspect_ratio;
		float halfWidth = width;
		halfWidth /= 2;
		ndcToScreen = Translate(halfWidth - widthScale - 0.5, 0, 0) * Scale(widthScale, heightScale, 1) * Translate(1, 1, 0) * ndcToScreen;
	}
	else {
		widthScale = width - 1;
		widthScale /= 2;
		heightScale = widthScale / camera_aspect_ratio;
		float halfHeight = height;
		halfHeight /= 2;
		ndcToScreen = Translate(0, halfHeight - heightScale - 0.5, 0) * Scale(widthScale, heightScale, 1) * Translate(1, 1, 0) * ndcToScreen;
	}
	return ndcToScreen;
}


Renderer::Renderer() :m_width(512), m_height(512)
{
	InitOpenGLRendering();
	CreateBuffers(512,512);
	ndcToScreen = CreateNdcToScreenMatrix(512, 512,1);
}
Renderer::Renderer(int width, int height) :m_width(width), m_height(height)
{
	InitOpenGLRendering();
	CreateBuffers(width, height);
	camera_aspect_ratio = width;
	camera_aspect_ratio /= height;
	if (antialiasing_mode)
		ndcToScreen = CreateNdcToScreenMatrix(width*2, height*2, camera_aspect_ratio);
	else
		ndcToScreen = CreateNdcToScreenMatrix(width, height, camera_aspect_ratio);
}

Renderer::~Renderer(void)
{
}

GLfloat Renderer::sumQuadPixels(int x, int y, const int& color) {
	float k = 0;
	x *= 2; 
	y *= 2;
	k += m_aa_outBuffer[INDEX(m_width * 2, x, y, color)];
	k += m_aa_outBuffer[INDEX(m_width * 2, x + 1, y, color)];
	k += m_aa_outBuffer[INDEX(m_width * 2, x, y + 1, color)];
	k += m_aa_outBuffer[INDEX(m_width * 2, x + 1, y + 1, color)];
	k /= 4;
	return k;
}

void Renderer::antiAlias() {
	for (int x = 0; x<m_width; ++x) {
		for (int y = 0; y<m_height; ++y) {
			m_outBuffer[INDEX(m_width, x, y, 0)] = sumQuadPixels(x, y, 0);
			m_outBuffer[INDEX(m_width, x, y, 1)] = sumQuadPixels(x, y, 1);
			m_outBuffer[INDEX(m_width, x, y, 2)] = sumQuadPixels(x, y, 2);
		}
	}
}

float Renderer::GetPixelZValue(int x, int y)
{
	if (antialiasing_mode) {
		return m_aa_zbuffer[x + y*m_width * 2];
	}
	else {
		return m_zbuffer[x + y*m_width];
	}
}

void Renderer::DrawPixel(int x, int y, float z, const vec4& color)
{
	if (antialiasing_mode)
	{
		if (m_aa_zbuffer[x + y*m_width*2] < z)
		{
			m_aa_zbuffer[x + y*m_width*2] = z;
			if (draw_fog)
			{
				vec4 c = this->fog->draw(z, color);
				m_aa_outBuffer[INDEX(m_width*2, x, y, 0)] = c.x;
				m_aa_outBuffer[INDEX(m_width*2, x, y, 1)] = c.y;
				m_aa_outBuffer[INDEX(m_width*2, x, y, 2)] = c.z;
			}
			else
			{
				m_aa_outBuffer[INDEX(m_width*2, x, y, 0)] = color.x;
				m_aa_outBuffer[INDEX(m_width*2, x, y, 1)] = color.y;
				m_aa_outBuffer[INDEX(m_width*2, x, y, 2)] = color.z;
			}
		}

	}
	else
	{
		if (m_zbuffer[x + y*m_width] < z)
		{
			m_zbuffer[x + y*m_width] = z;
			if (draw_fog)
			{
				vec4 c = this->fog->draw(z, color);
				m_outBuffer[INDEX(m_width, x, y, 0)] = c.x;
				m_outBuffer[INDEX(m_width, x, y, 1)] = c.y;
				m_outBuffer[INDEX(m_width, x, y, 2)] = c.z;
			}
			else
			{
				m_outBuffer[INDEX(m_width, x, y, 0)] = color.x;
				m_outBuffer[INDEX(m_width, x, y, 1)] = color.y;
				m_outBuffer[INDEX(m_width, x, y, 2)] = color.z;
			}
		}
	}
}

void CalculateMinAndMax(int a, int b, int c, int& min, int& max) {
	min = a;
	if (b < min) {
		min = b;
	}
	if (c < min) {
		min = c;
	}
	max = a;
	if (b > max) {
		max = b;
	}
	if (c > max) {
		max = c;
	}
}

void CalculateMinAndMax(int a, int b, int& min, int& max) {
	min = a;
	if (b < min) {
		min = b;
	}
	max = a;
	if (b > max) {
		max = b;
	}
}

void Renderer::CalculateScanLines(int x1, int y1, int x2, int y2, int x3, int y3, ScanLines& scanLines)
{
	CalculateMinAndMax(y1, y2, y3, scanLines.yMin, scanLines.yMax);
	scanLines.ResetLimits();
	BresenhamAlgorithm(x1, y1, x2, y2, scanLines);
	BresenhamAlgorithm(x2, y2, x3, y3, scanLines);
	BresenhamAlgorithm(x3, y3, x1, y1, scanLines);
}

void Renderer::BresenhamAlgorithm(int x1, int y1, int x2, int y2, ScanLines& scanLines)
{
	if (x1 == x2) {
		int yMin, yMax;
		CalculateMinAndMax(y1, y2, yMin, yMax);
		for (int y = yMin; y <= yMax; ++y) {
			scanLines.SetLimits(x1, y);
		}
	}
	else if (y1 == y2) {
		scanLines.SetLimits(x1, y1);
		scanLines.SetLimits(x2, y2);
	}
	else {
		int dx, dy;
		dx = x1 - x2;
		dy = y1 - y2;
		if (dx < 0) {
			dx *= -1;
			dy *= -1;
		}
		int slopeSign = dy * dx > 0 ? 1 : -1;
		bool iterateX = true;
		// if the slope m is:   m < -1   OR   1 < m
		if (dy < -dx || dx < dy) {
			iterateX = false;
		}
		// x here represents the iterate axis (NOT necessarily x)
		int dX, dY, dE, dNE;
		int xS, yS, xE, yE, d;
		if (iterateX) {
			if (x1 < x2) {
				xS = x1;
				yS = y1;
				xE = x2;
				yE = y2;
				dX = x2 - x1;
				dY = y2 - y1;
			}
			else {
				xS = x2;
				yS = y2;
				xE = x1;
				yE = y1;
				dX = x1 - x2;
				dY = y1 - y2;
			}
		}
		else {
			if (y1 < y2) {
				xS = y1;
				yS = x1;
				xE = y2;
				yE = x2;
				dX = y2 - y1;
				dY = x2 - x1;
			}
			else {
				xS = y2;
				yS = x2;
				xE = y1;
				yE = x1;
				dX = y1 - y2;
				dY = x1 - x2;
			}
		}
		if (dY < 0) {
			dY *= -1;
		}
		d = 2 * dY - dX;
		dE = 2 * dY;
		dNE = 2 * (dY - dX);
		if (iterateX) {
			scanLines.SetLimits(xS, yS);
		}
		else {
			scanLines.SetLimits(yS, xS);
		}
		while (xS < xE)
		{
			if (d < 0)
			{
				d += dE;
				++xS;
			}
			else {
				d += dNE;
				++xS;
				yS += slopeSign;
			}
			if (iterateX) {
				scanLines.SetLimits(xS, yS);
			}
			else {
				scanLines.SetLimits(yS, xS);
			}
		}
	}
}


void Renderer::CreateBuffers(int width, int height)
{
	m_width=width;
	m_height=height;	
	CreateOpenGLBuffer(); //Do not remove this line.
	m_outBuffer = new float[3*m_width*m_height];
	m_zbuffer = new float[m_width*m_height];
	m_aa_outBuffer = new float[3 * m_width*m_height*4];
	m_aa_zbuffer = new float[m_width*m_height*4];
}

void Renderer::SetCameraTransform(const mat4& cTransform)
{
	this->viewTransform = cTransform;
}

void Renderer::SetProjection(const mat4& projection)
{
	this->projection = projection;
}

void Renderer::SetObjectMatrices(const mat4& oTransform, const mat3& nTransform)
{
	this->oTransform = oTransform;
	this->nTransform = nTransform;
}

void Renderer::AdjustToCameraAspectRatio(float camera_aspect_ratio)
{
	this->camera_aspect_ratio = camera_aspect_ratio;
	if (antialiasing_mode)
		ndcToScreen = CreateNdcToScreenMatrix(m_width*2, m_height*2, camera_aspect_ratio);
	else
		ndcToScreen = CreateNdcToScreenMatrix(m_width, m_height, camera_aspect_ratio);
}

void Renderer::UpdateScreenSize(int width, int height)
{
	delete m_outBuffer;
	delete m_zbuffer;
	delete m_aa_outBuffer;
	delete m_aa_zbuffer;
	CreateBuffers(width, height);
	AdjustToCameraAspectRatio(this->camera_aspect_ratio);
}

void Renderer::SetLightSources(const vector<LightSource>* light_sources)
{
	lightSources = vector<LightSource>(*light_sources);
}

inline bool IsInsideNDC(const vec4& p) {
	return (-1 <= p.x && p.x <= 1 && -1 <= p.y && p.y <= 1 && -1 <= p.z && p.z <= 1);
}

void Renderer::ClearColorBuffer() {
	if (antialiasing_mode)
		memset(m_aa_outBuffer, 0, 3 * m_width * m_height*4 * sizeof(float));
	else
		memset(m_outBuffer, 0, 3 * m_width * m_height * sizeof(float));
}

void Renderer::ClearDepthBuffer() {
	if (antialiasing_mode)
		for (int y = 0; y < m_height*2; ++y) {
			for (int x = 0; x < m_width*2; ++x) {
				m_aa_zbuffer[y * m_width*2 + x] = -FLT_MAX;
			}
		}
	else
		for (int y = 0; y < m_height; ++y) {
			for (int x = 0; x < m_width; ++x) {
				m_zbuffer[y * m_width + x] = -FLT_MAX;
			}
		}
}

void Renderer::ShadingColor(const vec3& p, const vec3& eye, const vec3& n, const Material& material, vec4& color)
{
	vec3 v = eye;
	v -= p;
	vec3 l, half;
	vec4 noColor(0.0, 0.0, 0.0, 1.0), ambient, diffuse, specular;
	color = noColor;
	float d, s, distance, distanceAttenuation;
	for (int i = 0; i < lightSources.size(); ++i) {
		LightSource& lightSource = lightSources[i];
		l = vec3(lightSource.position.x, lightSource.position.y, lightSource.position.z);
		switch (lightSource.sourceType) {
		case POINT_LIGHT:
			l -= p;
			distance = length(l);
			distanceAttenuation = lightSource.constantAttenuation;
			distanceAttenuation += lightSource.linearAttenuation * distance;
			distanceAttenuation += lightSource.quadraticAttenuation * (distance * distance);
			distanceAttenuation = 1 / distanceAttenuation;
			break;
		case PARALLEL_LIGHT:
			l *= -1;
			break;
		}
		ambient = lightSource.ambient;
		ambient *= material.ambient;
		d = dot(n, l);
		if (d > 0) {
			diffuse = lightSource.diffuse;
			diffuse *= material.diffuse;
			diffuse *= (d / length(l));
			half = l;
			half += v;
			s = dot(half, n);
			if (s > 0.0) {
				specular = lightSource.specular;
				specular *= material.specular;
				specular *= pow(s / length(half), material.shininess);
			}
			else {
				specular = noColor;
			}
		}
		else {
			diffuse = noColor;
			specular = noColor;
		}
		if (lightSource.sourceType == POINT_LIGHT) {
			diffuse *= distanceAttenuation;
			specular *= distanceAttenuation;
		}
		color += ambient;
		color += diffuse;
		color += specular;
	}
	color += material.emission;
	if (color.x > 1) {
		color.x = 1.0;
	}
	if (color.y > 1) {
		color.y = 1.0;
	}
	if (color.z > 1) {
		color.z = 1.0;
	}
	color.w = 1.0;
}


void Renderer::DrawTriangles(const vector<vec3>* vertices,
	const vector<vec3>* v_normals, const vector<vec3>* f_normals, COLORS color, const ModelMaterial* material)
{
	ScanLines scanLines(m_height);
	// Build the transform matrix
	mat4 transform(projection);
	transform.multiply(viewTransform);
	transform.multiply(oTransform);
	// Draw all the triangles
	vec4 pixelColor = vec4(0.5, 0.9, 0.9, 1);
	vec4 uColor, vColor, wColor;
	switch (color) {
	case WHITE:
		pixelColor = { 1, 1, 1, 1 };
		break;
	case YELLOW:
		pixelColor = { 1, 1, 0, 1 };
		break;
	case BLUE:
		pixelColor = { 0, 0, 1, 1 };
		break;
	}


	float z = 0;
	float p1z, p2z, p3z;
	vec3 weights;
	int x1, y1, x2, y2, x3, y3;
	for (int i = 0; i < vertices->size(); i += 3) {
		vec4 v1 = (*vertices)[i];
		vec4 v2 = (*vertices)[i + 1];
		vec4 v3 = (*vertices)[i + 2];
		vec4 p1 = oTransform * v1;
		vec4 p2 = oTransform * v2;
		vec4 p3 = oTransform * v3;
		vec3 w1 = vec3(p1.x, p1.y, p1.z);
		vec3 w2 = vec3(p2.x, p2.y, p2.z);
		vec3 w3 = vec3(p3.x, p3.y, p3.z);
		p1 = transform * v1;
		p2 = transform * v2;
		p3 = transform * v3;
		p1z = p1.z;
		p2z = p2.z;
		p3z = p3.z;
		p1 /= p1.w;
		p2 /= p2.w;
		p3 /= p3.w;

		// doing some very tough clipping...
		if (IsInsideNDC(p1) && IsInsideNDC(p2) && IsInsideNDC(p3)) {
			p1 = ndcToScreen * p1;
			p2 = ndcToScreen * p2;
			p3 = ndcToScreen * p3;
			if (shadingType == FLAT_SHADING || !v_normals) {
				vec3 avgPosition(w1);
				avgPosition += w2;
				avgPosition += w3;
				avgPosition /= 3;
				vec3 normal = normalize(cross(w3 - w2, w1 - w2));
				ShadingColor(avgPosition, eye, normal, material->materials[0], pixelColor);
			}
			x1 = p1[0];
			y1 = p1[1];
			x2 = p2[0];
			y2 = p2[1];
			x3 = p3[0];
			y3 = p3[1];
			CalculateScanLines(x1, y1, x2, y2, x3, y3, scanLines);
			Triangle2D triangle(x1, y1, x2, y2, x3, y3);
			if (shadingType == GOURAUD_SHADING && v_normals) {
				vec3 n1 = nTransform * (*v_normals)[i];
				vec3 n2 = nTransform * (*v_normals)[i + 1];
				vec3 n3 = nTransform * (*v_normals)[i + 2];
				ShadingColor(w1, eye, n1, material->materials[0], uColor);
				ShadingColor(w2, eye, n2, material->materials[1], vColor);
				ShadingColor(w3, eye, n3, material->materials[2], wColor);
				for (int y = scanLines.yMin; y <= scanLines.yMax; ++y) {
					for (int x = scanLines.xLimits[2 * y]; x <= scanLines.xLimits[2 * y + 1]; ++x) {
						triangle.Barycentric(x, y, weights);
						z = weights.x * p1z + weights.y * p2z + weights.z * p3z;
						if (z <= GetPixelZValue(x, y)) {
							continue;
						}
						pixelColor = uColor * weights.x;
						pixelColor += vColor * weights.y;
						pixelColor += wColor * weights.z;
						DrawPixel(x, y, z, pixelColor);
					}
				}
			}
			else if (shadingType == PHONG_SHADING && v_normals) {
				vec3 n1 = nTransform * (*v_normals)[i];
				vec3 n2 = nTransform * (*v_normals)[i + 1];
				vec3 n3 = nTransform * (*v_normals)[i + 2];
				for (int y = scanLines.yMin; y <= scanLines.yMax; ++y) {
					for (int x = scanLines.xLimits[2 * y]; x <= scanLines.xLimits[2 * y + 1]; ++x) {
						triangle.Barycentric(x, y, weights);
						z = weights.x * p1z + weights.y * p2z + weights.z * p3z;
						if (z <= GetPixelZValue(x, y)) {
							continue;
						}
						vec3 point = weights.x * w1 + weights.y * w2 + weights.z * w3;
						vec3 pointNormal = weights.x * n1 + weights.y * n2 + weights.z * n3;
						if (material->uniform) {
							ShadingColor(point, eye, pointNormal, material->materials[0], pixelColor);
						}
						else {
							Material avgMaterial = material->materials[0];
							avgMaterial *= weights.x;
							avgMaterial += material->materials[1] * weights.y;
							avgMaterial += material->materials[2] * weights.z;
							ShadingColor(point, eye, pointNormal, avgMaterial, pixelColor);
						}
						DrawPixel(x, y, z, pixelColor);
					}
				}
			}
			else {
				for (int y = scanLines.yMin; y <= scanLines.yMax; ++y) {
					for (int x = scanLines.xLimits[2 * y]; x <= scanLines.xLimits[2 * y + 1]; ++x) {
						triangle.Barycentric(x, y, weights);
						z = weights.x * p1z + weights.y * p2z + weights.z * p3z;
						DrawPixel(x, y, z, pixelColor);
					}
				}
			}
		}
	}
}



inline bool Renderer::IsInsideScreen(int x, int y)
{
	if (antialiasing_mode)
		return (0 <= x && x < m_width*2 && 0 <= y && y < m_height*2);
	else
		return (0 <= x && x < m_width && 0 <= y && y < m_height);
}


void Renderer::SetShadingType(SHADING_TYPES shading)
{
	this->shadingType = shading;
}

void Renderer::SetEye(const vec3& eye)
{
	this->eye = eye;
}

/////////////////////////////////////////////////////
//OpenGL stuff. Don't touch.

void Renderer::InitOpenGLRendering()
{
	int a = glGetError();
	a = glGetError();
	glGenTextures(1, &gScreenTex);
	a = glGetError();
	glGenVertexArrays(1, &gScreenVtc);
	GLuint buffer;
	glBindVertexArray(gScreenVtc);
	glGenBuffers(1, &buffer);
	const GLfloat vtc[]={
		-1, -1,
		1, -1,
		-1, 1,
		-1, 1,
		1, -1,
		1, 1
	};
	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);
	glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);
	glBufferSubData( GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );
	glUseProgram( program );
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );

	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition, 2, GL_FLOAT, GL_FALSE, 0,
		0 );

	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
		(GLvoid *) sizeof(vtc) );
	glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );
	a = glGetError();
}

void Renderer::CreateOpenGLBuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, m_width, m_height, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, m_width, m_height);
}

void Renderer::SwapBuffers()
{
	if (antialiasing_mode)
	{
		antiAlias();
	}
		
	int a = glGetError();
	glActiveTexture(GL_TEXTURE0);
	a = glGetError();
	glBindTexture(GL_TEXTURE_2D, gScreenTex);
	a = glGetError();
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_FLOAT, m_outBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	a = glGetError();

	glBindVertexArray(gScreenVtc);
	a = glGetError();
	glDrawArrays(GL_TRIANGLES, 0, 6);
	a = glGetError();
	glutSwapBuffers();
	a = glGetError();
}

void Renderer::DrawBoundingBox(const vector<vec3>* bounding_box, COLORS color)
{
	vector<vec3> ver;
	vector<vec3> b = *bounding_box;
	GLfloat min_x = b[0].x;
	GLfloat min_y = b[0].y;
	GLfloat min_z = b[0].z;
	GLfloat max_x = b[1].x;
	GLfloat max_y = b[1].y;
	GLfloat max_z = b[1].z;
	//line 1
	ver.push_back(vec3(min_x, min_y, min_z));
	ver.push_back(vec3(min_x, min_y, min_z));
	ver.push_back(vec3(min_x, max_y, min_z));
	//line 2
	ver.push_back(vec3(min_x, min_y, min_z));
	ver.push_back(vec3(min_x, min_y, min_z));
	ver.push_back(vec3(max_x, min_y, min_z));
	//line 3
	ver.push_back(vec3(max_x, max_y, min_z));
	ver.push_back(vec3(max_x, max_y, min_z));
	ver.push_back(vec3(min_x, max_y, min_z));
	//line 4
	ver.push_back(vec3(max_x, max_y, min_z));
	ver.push_back(vec3(max_x, max_y, min_z));
	ver.push_back(vec3(max_x, min_y, min_z));
	//line 5
	ver.push_back(vec3(min_x, min_y, max_z));
	ver.push_back(vec3(min_x, min_y, max_z));
	ver.push_back(vec3(min_x, max_y, max_z));
	//line 6
	ver.push_back(vec3(min_x, min_y, max_z));
	ver.push_back(vec3(min_x, min_y, max_z));
	ver.push_back(vec3(max_x, min_y, max_z));
	//line 7
	ver.push_back(vec3(max_x, max_y, max_z));
	ver.push_back(vec3(max_x, max_y, max_z));
	ver.push_back(vec3(min_x, max_y, max_z));
	//line 8
	ver.push_back(vec3(max_x, max_y, max_z));
	ver.push_back(vec3(max_x, max_y, max_z));
	ver.push_back(vec3(max_x, min_y, max_z));
	//line 9
	ver.push_back(vec3(min_x, min_y, min_z));
	ver.push_back(vec3(min_x, min_y, min_z));
	ver.push_back(vec3(min_x, min_y, max_z));
	//line 10
	ver.push_back(vec3(min_x, max_y, min_z));
	ver.push_back(vec3(min_x, max_y, min_z));
	ver.push_back(vec3(min_x, max_y, max_z));
	//line 3
	ver.push_back(vec3(max_x, max_y, min_z));
	ver.push_back(vec3(max_x, max_y, min_z));
	ver.push_back(vec3(max_x, max_y, max_z));
	//line 4
	ver.push_back(vec3(max_x, min_y, min_z));
	ver.push_back(vec3(max_x, min_y, min_z));
	ver.push_back(vec3(max_x, min_y, max_z));
	this->DrawTriangles(&ver, NULL, NULL, color);
}

void Renderer::DrawFaceNormals(const vector<vec3>* f_normals)
{

	vec3 p1, p2,norm;
	vec4 tail, end;
	GLfloat z = 0;
	vec4 defaultColor = vec4(1, 0, 0, 1); //red
	// Build the transform matrix
	mat4 transform(projection);
	transform.multiply(viewTransform);
	for (int i = 0; i < f_normals->size() / 2; i++) {
		p1 = vec3((*f_normals)[2 * i]);
		p2 = vec3((*f_normals)[2 * i + 1]);
		norm = normal_length*normalize(this->nTransform * (p2 - p1));
		tail = oTransform * vec4(p1, 1);
		end = tail + vec4(norm, 0);
		tail = transform * tail;
		end = transform * end;
		GLfloat p1z = tail.z;
		GLfloat p2z = end.z;
		GLfloat p3z = end.z;
		tail /= tail.w;
		end /= end.w;
		bool p1InsideNDC = IsInsideNDC(tail);
		bool p2InsideNDC = IsInsideNDC(end);
		bool p3InsideNDC = IsInsideNDC(end);
		tail = ndcToScreen * tail;
		end = ndcToScreen * end;
		ScanLines scanLines(m_height);
		vec4 p1 = tail, p2 = end, p3 = end;
		vec3 _p1 = vec3(p1.x, p1.y, p1z);
		vec3 _p2 = vec3(p2.x, p2.y, p2z);
		vec3 _p3 = vec3(p3.x, p3.y, p3z);
		if (p1InsideNDC && p2InsideNDC && p3InsideNDC) {
			CalculateScanLines(p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], scanLines);
			for (int y = scanLines.yMin; y <= scanLines.yMax; ++y) {
				for (int x = scanLines.xLimits[2 * y]; x <= scanLines.xLimits[2 * y + 1]; ++x) {

					//z interpolation using plane 3D/line for triangle/grid correspondingly
					GLfloat tx = (x - _p1.x) / (_p3.x - _p1.x); //3D line interpolation calculation.
					z = _p1.z + (tx*(_p3.z - _p1.z));
					DrawPixel(x, y, z, defaultColor);
				}
			}
		}
	}
}

void Renderer::DrawVertexNormals(const vector<vec3>* vertices,const vector<vec3>* v_normals)
{
	vec3 norm;
	vec4 tail, end, p;
	GLfloat z = 0;
	vec4 defaultColor = vec4(1, 0, 0, 1);
	// Build the transform matrix
	mat4 transform(projection);
	transform.multiply(viewTransform);
	for (int i = 0; i < vertices->size(); i++) {
		norm = normal_length*normalize(this->nTransform * ((*v_normals)[i]));
		p = vec4((*vertices)[i], 1);
		tail = oTransform * p;
		end = tail + vec4(norm, 0);
		tail = transform * tail;
		end = transform * end;
		GLfloat p1z = tail.z;
		GLfloat p2z = end.z;
		GLfloat p3z = end.z;
		tail /= tail.w;
		end /= end.w;
		bool p1InsideNDC = IsInsideNDC(tail);
		bool p2InsideNDC = IsInsideNDC(end);
		bool p3InsideNDC = IsInsideNDC(end);
		tail = ndcToScreen * tail;
		end = ndcToScreen * end;
		//if (tailInsideNDC && endInsideNDC)
		//{
		//	DrawLine(tail[0], tail[1], end[0], end[1], RED);
		//}
		ScanLines scanLines(m_height);
		vec4 p1=tail, p2=end, p3=end;
		vec3 _p1 = vec3(p1.x, p1.y, p1z);
		vec3 _p2 = vec3(p2.x, p2.y, p2z);
		vec3 _p3 = vec3(p3.x, p3.y, p3z);

		if (p1InsideNDC && p2InsideNDC && p3InsideNDC) {
			CalculateScanLines(p1[0], p1[1], p2[0], p2[1], p3[0], p3[1], scanLines);
			for (int y = scanLines.yMin; y <= scanLines.yMax; ++y) {
				for (int x = scanLines.xLimits[2 * y]; x <= scanLines.xLimits[2 * y + 1]; ++x) {

					//z interpolation using plane 3D/line for triangle/grid correspondingly
						GLfloat tx = (x - _p1.x) / (_p3.x - _p1.x); //3D line interpolation calculation.
						z = _p1.z + (tx*(_p3.z - _p1.z));
					DrawPixel(x, y, z, defaultColor);
				}
			}
		}
			
	}
}