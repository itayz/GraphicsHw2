#pragma once
#include "MeshModel.h"
#include "Renderer.h"
class PrimMeshModel : public MeshModel
{
private:
	enum PRIMITIVE_MODELS //PYRAMID=1 
	{
		PYRAMID = 1,
		XZ_GRID = 2
	};
	PRIMITIVE_MODELS primitive_type;
public:
	PrimMeshModel(int xMin, int xMax, int zMin, int zMax); //WorldGrid
	PrimMeshModel(const int s=1); //pyramid
	~PrimMeshModel();
	void draw(Renderer& renderer, COLORS color = CYAN);
};