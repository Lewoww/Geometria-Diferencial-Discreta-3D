#pragma once
#include "Vertex3d.h"
class TriangleFace
{
private: 
	Vertex3d* v1;
	Vertex3d* v2;
	Vertex3d* v3;

	float r, g, k;

public:
	TriangleFace(Vertex3d* a, Vertex3d* b, Vertex3d* c);

	Vertex3d get_V1() const;
	Vertex3d get_V2() const;
	Vertex3d get_V3() const;
	float get_r() const;
	float get_g() const;
	float get_b() const;
};

