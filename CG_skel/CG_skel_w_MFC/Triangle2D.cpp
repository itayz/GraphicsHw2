#include "stdafx.h"
#include "Triangle2D.h"

Triangle2D::Triangle2D(int x1, int y1, int x2, int y2, int x3, int y3) : x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {
	v0x = x2 - x1;
	v0y = y2 - y1;
	v1x = x3 - x1;
	v1y = y3 - y1;
	den = v0x * v1y - v1x * v0y;
	den = 1 / den;
}

void Triangle2D::Barycentric(const int& x, const int& y, float& u, float& v, float& w) {
	int v2x = x - x1;
	int v2y = y - y1;
	v = den * (v2x * v1y - v1x * v2y);
	w = den * (v0x * v2y - v2x * v0y);
	u = 1.0 - v - w;
}