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
		if (x == x1 && y == y1) {
			if (x == x2 && y == y2) {
				weights.x = 0.5;
				weights.y = 0.5;
				weights.z = 0.0;
			}
			else if (x == x3 && y == y3) {
				weights.x = 0.5;
				weights.y = 0.0;
				weights.z = 0.5;
			}
			else {
				weights.x = 1.0;
				weights.y = 0.0;
				weights.z = 0.0;
			}
		}
		else if (x == x2 && y == y2) {
			if (x == x3 && y == y3) {
				weights.x = 0.0;
				weights.y = 0.5;
				weights.z = 0.5;
			}
			else {
				weights.x = 0.0;
				weights.y = 1.0;
				weights.z = 0.0;
			}
		}
		else if (x == x3 && y == y3) {
			weights.x = 0.0;
			weights.y = 0.0;
			weights.z = 1.0;
		}
		else {
			float w1, w2, w3;
			w1 = abs(x1 - x) + abs(y1 - y);
			w1 = 1 / w1;
			w2 = abs(x2 - x) + abs(y2 - y);
			w2 = 1 / w2;
			w3 = abs(x3 - x) + abs(y3 - y);
			w3 = 1 / w3;
			float inverseTotalWeights = w1 + w2 + w3;
			inverseTotalWeights = 1 / inverseTotalWeights;
			weights.x = w1 * inverseTotalWeights;
			weights.y = w2 * inverseTotalWeights;
			weights.z = 1.0 - weights.x - weights.y;
		}
	}
	else {
		weights.y = 1;
		weights.y /= 3;
		weights.z = weights.y;
		weights.x = 1.0 - weights.y - weights.z;
	}
}