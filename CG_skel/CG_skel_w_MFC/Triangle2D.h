#include "vec.h"

class Triangle2D
{
	int x1, y1, x2, y2, x3, y3;
	bool isPoint, isLine, isTriangle;
	int v0x, v0y, v1x, v1y, den;
	float inverseDen;

public:
	Triangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
	void Barycentric(int x, int y, vec3& weights);
};