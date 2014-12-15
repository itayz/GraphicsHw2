#include "stdafx.h"
#include "Triangle2D.h"

Triangle2D::Triangle2D(int x1, int y1, int x2, int y2, int x3, int y3)
	: x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3), isPoint(false), isLine(false), isTriangle(true) {
	v0x = x2 - x1;
	v0y = y2 - y1;
	v1x = x3 - x1;
	v1y = y3 - y1;
	den = v0x * v1y - v1x * v0y;
	if (den != 0) {
		inverseDen = den;
		inverseDen = 1 / inverseDen;
	}
	else {
		isTriangle = false;
		if (x1 != x2 || x2 != x3 || y1 != y2 || y2 != y3) {
			isLine = true;
			int v2x = x2 - x3;
			int v2y = y2 - y3;
			inverseLineLength = sqrt(v0x * v0x + v0y * v0y) + sqrt(v1x * v1x + v1y * v1y) + sqrt(v2x * v2x + v2y * v2y);
			inverseLineLength = 2 / inverseLineLength;
		}
		else {
			isPoint = true;
		}
	}
}

void Triangle2D::Barycentric(int x, int y, vec3& weights) {
	if (isTriangle) {
		int v2x = x - x1;
		int v2y = y - y1;
		weights.y = inverseDen * (v2x * v1y - v1x * v2y);
		weights.z = inverseDen * (v0x * v2y - v2x * v0y);
		weights.x = 1.0 - weights.y - weights.z;
	}
	else if (isLine) {
		weights.x = 1 - inverseLineLength * sqrt((x1 - x) * (x1 - x) + (y1 - y) * (y1 - y));
		weights.y = 1 - inverseLineLength * sqrt((x2 - x) * (x2 - x) + (y2 - y) * (y2 - y));
		weights.z = 1 - inverseLineLength * sqrt((x3 - x) * (x3 - x) + (y3 - y) * (y3 - y));
		weights /= length(weights);
	}
	else {
		weights.y = 1;
		weights.y /= 3;
		weights.z = weights.y;
		weights.x = 1.0 - weights.y - weights.z;
	}
}