class Triangle2D
{
	int x1, y1, x2, y2, x3, y3;
	int v0x, v0y, v1x, v1y;
	float den;

public:
	Triangle2D(int x1, int y1, int x2, int y2, int x3, int y3);
	void Barycentric(int x, int y, float& u, float& v, float& w);
};