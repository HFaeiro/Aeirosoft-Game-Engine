#pragma once
#include "framework.h"


class Vertex
{
public:

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	Vertex();
	Vertex(DirectX::XMFLOAT3 f3, DirectX::XMFLOAT2 f2)
		:position(f3),
		texture(f2)
	{}
	Vertex(float f1, float f2, float f3, float f4, float f5)
		:position(DirectX::XMFLOAT3(f1, f2, f3)),
		texture(DirectX::XMFLOAT2(f4, f5))
	{}

};


