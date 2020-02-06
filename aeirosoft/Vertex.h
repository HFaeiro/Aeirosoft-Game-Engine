#pragma once
#include "framework.h"
#include "Inc/DirectX/SimpleMath.h"
#include "Inc/Assimp/Vertex.h"
class Vertex
{
public:

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	
	Vertex() {}
	Vertex(DirectX::XMFLOAT3 f3, DirectX::XMFLOAT2 f2)
		:position(f3),
		texture(f2)
	{}
	Vertex(float f1, float f2, float f3, float f4, float f5)
		:position(DirectX::XMFLOAT3(f1, f2, f3)),
		texture(DirectX::XMFLOAT2(f4, f5))
	{}
	Vertex(float f1, float f2, float f3)
		:position(DirectX::XMFLOAT3(f1, f2, f3)),
		texture(DirectX::XMFLOAT2(0,0))
	{}

	Vertex(aiVector3D aiV, aiVector3D aiT)
		:position(DirectX::XMFLOAT3(aiV.x, aiV.y, aiV.z)),
		texture(DirectX::XMFLOAT2(aiT.x, aiT.y))
	{}
};




