#pragma once
#include "framework.h"
#include "Inc/DirectX/SimpleMath.h"
#include "Inc/Assimp/Vertex.h"
class Vertex
{
public:

	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
	BYTE Indecies[4] = { 0 };
	float weights[4] = { 0 };



	Vertex() {}
	Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT2 tex)
		:position(pos),
		texture(tex),
		normal({ 0, 0.f, 1.f })
	{}
	Vertex(float f1, float f2, float f3, float f4, float f5, float f6, float f7, float f8)
		:position(DirectX::XMFLOAT3(f1, f2, f3)),
		texture(DirectX::XMFLOAT2(f4, f5)),
		normal({f6, f7, f8})
	{}
	Vertex(float f1, float f2, float f3, float f4, float f5)
		:position(DirectX::XMFLOAT3(f1, f2, f3)),
		texture(DirectX::XMFLOAT2(f4, f5)),
		normal({ 1.0, 0, 0.f })
	{}
	Vertex(float f1, float f2, float f3)
		:position(DirectX::XMFLOAT3(f1, f2, f3)),
		texture(DirectX::XMFLOAT2(0,0)),
		normal({0, 0, -1.f })
	{}

	Vertex(aiVector3D aiV, aiVector3D aiT)
		:position(DirectX::XMFLOAT3(aiV.x, aiV.y, aiV.z)),
		texture(DirectX::XMFLOAT2(aiT.x, aiT.y)),
		normal({ 0, 0, -1.f })
	{}
	void AddWeights(UINT index, float weight)
	{
		for (int i = 0; i < 4; i++)
		{
			if (weights[i] == 0.f)
			{
				Indecies[i] = index;
				weights[i] = weight;
				return;
			}
		}
		//should never get here. using instruction as breakpoint.
		int i = 1; 
		return;
	}
};




