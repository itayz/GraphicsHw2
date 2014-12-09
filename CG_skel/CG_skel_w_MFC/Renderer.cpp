#include "StdAfx.h"
#include "Renderer.h"
#include "CG_skel_w_MFC.h"
#include "InitShader.h"
#include "GL\freeglut.h"

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
	ndcToScreen = CreateNdcToScreenMatrix(width, height, camera_aspect_ratio);
}

Renderer::~Renderer(void)
{
}



void Renderer::CreateBuffers(int width, int height)
{
	m_width=width;
	m_height=height;	
	CreateOpenGLBuffer(); //Do not remove this line.
	m_outBuffer = new float[3*m_width*m_height];
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
	ndcToScreen = CreateNdcToScreenMatrix(m_width, m_height, camera_aspect_ratio);
}

void Renderer::UpdateScreenSize(int width, int height)
{
	delete m_outBuffer;
	CreateBuffers(width, height);
	AdjustToCameraAspectRatio(this->camera_aspect_ratio);
}

inline bool IsInsideNDC(const vec4& p) {
	return (-1 <= p.x && p.x <= 1 && -1 <= p.y && p.y <= 1 && -1 <= p.z && p.z <= 1);
}

void Renderer::ClearColorBuffer() {
	memset(m_outBuffer, 0, 3 * m_width * m_height * sizeof(float));
}

void Renderer::DrawTriangles(const vector<vec3>* vertices,
	const vector<vec3>* v_normals, const vector<vec3>* f_normals, COLORS color)
{
	// Build the transform matrix
	mat4 transform(projection);
	transform.multiply(viewTransform);
	transform.multiply(oTransform);
	// Draw all the triangles
	vec4 p1, p2, p3;
	for (int i = 0; i < vertices->size() / 3; i++) {
		p1 = vec4((*vertices)[3*i]);
		p2 = vec4((*vertices)[3*i+1]);
		p3 = vec4((*vertices)[3*i+2]);
		p1 = transform * p1;
		p2 = transform * p2;
		p3 = transform * p3;
		p1 /= p1.w;
		p2 /= p2.w;
		p3 /= p3.w;
		bool p1InsideNDC = IsInsideNDC(p1);
		bool p2InsideNDC = IsInsideNDC(p2);
		bool p3InsideNDC = IsInsideNDC(p3);
		p1 = ndcToScreen * p1;
		p2 = ndcToScreen * p2;
		p3 = ndcToScreen * p3;
		if (p1InsideNDC || p2InsideNDC)
			DrawLine(p1[0], p1[1], p2[0], p2[1], color);
		if (p3InsideNDC || p2InsideNDC)
			DrawLine(p3[0], p3[1], p2[0], p2[1], color);
		if (p1InsideNDC || p3InsideNDC)
			DrawLine(p1[0], p1[1], p3[0], p3[1], color);
	}
}

void Renderer::PlotPixel(int x, int y, COLORS color)
{
	int pixel = INDEX(m_width, x, y, 0);
	switch (color) {
	case WHITE:
		m_outBuffer[pixel] = 1;
		m_outBuffer[pixel + 1] = 1;
		m_outBuffer[pixel + 2] = 1;
		break;
	case BLACK:
		m_outBuffer[pixel] = 0;
		m_outBuffer[pixel + 1] = 0;
		m_outBuffer[pixel + 2] = 0;
		break;
	case RED:
		m_outBuffer[pixel] = 1;
		m_outBuffer[pixel + 1] = 0;
		m_outBuffer[pixel + 2] = 0;
		break;
	case GREEN:
	m_outBuffer[pixel] = 0;
	m_outBuffer[pixel + 1] = 1;
		m_outBuffer[pixel + 2] = 0;
		break;
	case BLUE:
		m_outBuffer[pixel] = 0;
		m_outBuffer[pixel + 1] = 0;
	m_outBuffer[pixel + 2] = 1;
		break;
	case YELLOW:
		m_outBuffer[pixel] = 1;
		m_outBuffer[pixel + 1] = 1;
		m_outBuffer[pixel + 2] = 0;
		break;
	case MAGNETA:
		m_outBuffer[pixel] = 1;
		m_outBuffer[pixel + 1] = 0;
		m_outBuffer[pixel + 2] = 1;
		break;
	case CYAN:
		m_outBuffer[pixel] = 0;
		m_outBuffer[pixel + 1] = 1;
		m_outBuffer[pixel + 2] = 1;
		break;
}
}

inline bool Renderer::IsInsideScreen(int x, int y)
{
	return (0 <= x && x < m_width && 0 <= y && y < m_height);
}

// draws a line using Bresenham algorithm
void Renderer::DrawLine(int x1, int y1, int x2, int y2, COLORS color)
{
	if ((x1 < 0 && x2 < 0) || (x1 >= m_width && x2 >= m_width)
		|| (y1 < 0 && y2 < 0) || (y1 >= m_height && y2 >= m_height)) {
		return;
	}
	int x[2], y[2];
	int dx = x1 - x2;
	int dy = y1 - y2;
	int pointsInScreen = 0;

	// in case we're dealing with a straight line.
	if (dx == 0) {
		int yLow, yHigh;
		yLow = y1 < y2 ? y1 : y2;
		yHigh = y1 + y2 - yLow;
		if (yLow < 0) {
			yLow = 0;
		}
		if (yHigh >= m_height) {
			yHigh = m_height - 1;
		}
		for (int i = yLow; i <= yHigh; ++i) {
			PlotPixel(x1, i, color);
		}
		return;
	}
	else if (dy == 0) {
		int xLow, xHigh;
		xLow = x1 < x2 ? x1 : x2;
		xHigh = x1 + x2 - xLow;
		if (xLow < 0) {
			xLow = 0;
		}
		if (xHigh >= m_width) {
			xHigh = m_width - 1;
		}
		for (int i = xLow; i <= xHigh; ++i) {
			PlotPixel(i, y1, color);
		}
		return;
	}

	// calculating the correct end points for the relevant line segment.
	if (IsInsideScreen(x1, y1)) {
		x[pointsInScreen] = x1;
		y[pointsInScreen] = y1;
		++pointsInScreen;
	}
	if (IsInsideScreen(x2, y2)) {
		x[pointsInScreen] = x2;
		y[pointsInScreen] = y2;
		//we make sure visible point is the starting point of the line//
		int tmp1 = x1;
		int tmp2 = y1;
		x1 = x2;
		y1 = y2;
		x2 = tmp1;
		y2 = tmp2;
		dx = x1 - x2;
		dy = y1 - y2;
		++pointsInScreen;
	}

	if (dx < 0) {
		dx *= -1;
		dy *= -1;
	}
	int temp;
	temp = dx * y2 - dy * x2; // temp/dx = n 
	if (pointsInScreen < 2 && 0 <= temp && temp < dx * m_height && x2<x1) { //left boundry
		x[pointsInScreen] = 0;
		y[pointsInScreen] = temp / dx;
		++pointsInScreen;
	}
	temp = dx * y2 + dy * (m_width - 1 - x2);
	if (pointsInScreen < 2 && 0 <= temp && temp < dx * m_height &&x2>x1) { //right boundry
		x[pointsInScreen] = m_width - 1;
		y[pointsInScreen] = temp / dx;
		++pointsInScreen;
	}
	if (dy < 0) {
		dx *= -1;
		dy *= -1;
	}
	temp = dy * x2 - dx * y2;
	if (pointsInScreen < 2 && 0 <= temp && temp < dy * m_width && y2<y1) { //bottom boundry
		x[pointsInScreen] = temp / dy;
		y[pointsInScreen] = 0;
		++pointsInScreen;
	}
	temp = dy * x2 + dx * (m_height - 1 - y2);
	if (pointsInScreen < 2 && 0 <= temp && temp < dy * m_width &&y2>y1) { //top boundry
		x[pointsInScreen] = temp / dy;
		y[pointsInScreen] = m_height - 1;
		++pointsInScreen;
	}
	if (pointsInScreen < 2) {
		return;
	}

	// since we got here it means we have something to draw.
	if (dx < 0) {
		dx *= -1;
		dy *= -1;
	}
	int slopeSign = dy * dx > 0 ? 1 : -1;
	bool iterateX = true;
	// if the slope m is:   -1 <= m <= 1
	if (-dx <= dy && dy <= dx) {
		if (x[0] < x[1]) {
			x1 = x[0];
			y1 = y[0];
			x2 = x[1];
			y2 = y[1];
		}
		else {
			x1 = x[1];
			y1 = y[1];
			x2 = x[0];
			y2 = y[0];
		}
	}
	// if the slope m is:    m < -1   OR   1 < m
	else {
		iterateX = false;
		if (dy < 0) {
			dx *= -1;
			dy *= -1;
		}
		if (y[0] < y[1]) {
			x1 = x[0];
			y1 = y[0];
			x2 = x[1];
			y2 = y[1];
		}
		else {
			x1 = x[1];
			y1 = y[1];
			x2 = x[0];
			y2 = y[0];
		}
	}

	// x here represents the iterate axis (NOT necessarily x)
	int dX, dY, dE, dNE;
	int xS, yS, xE, yE, d;
	if (iterateX) {
		xS = x1;
		yS = y1;
		xE = x2;
		yE = y2;
		dX = x2 - x1;
		dY = y2 - y1;
	}
	else {
		xS = y1;
		yS = x1;
		xE = y2;
		yE = x2;
		dX = y2 - y1;
		dY = x2 - x1;
	}
	if (dY < 0) {
		dY *= -1;
	}
	d = 2 * dY - dX;
	dE = 2 * dY;
	dNE = 2 * (dY - dX);
	if (iterateX) {
		PlotPixel(xS, yS, color);
	}
	else {
		PlotPixel(yS, xS, color);
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
			PlotPixel(xS, yS, color);
		}
		else {
			PlotPixel(yS, xS, color);
		}
	}
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
		tail /= tail.w;
		end /= end.w;
		bool tailInsideNDC = IsInsideNDC(tail);
		bool endInsideNDC = IsInsideNDC(end);
		tail = ndcToScreen * tail;
		end = ndcToScreen * end;
		if (tailInsideNDC || endInsideNDC)
			DrawLine(tail[0], tail[1], end[0], end[1],RED);
	}
}

void Renderer::DrawVertexNormals(const vector<vec3>* vertices,const vector<vec3>* v_normals)
{
	vec3 norm;
	vec4 tail, end, p;
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
		tail /= tail.w;
		end /= end.w;
		bool tailInsideNDC = IsInsideNDC(tail);
		bool endInsideNDC = IsInsideNDC(end);
		tail = ndcToScreen * tail;
		end = ndcToScreen * end;
		if (tailInsideNDC || endInsideNDC)
			DrawLine(tail[0], tail[1], end[0], end[1],RED);
	}
}