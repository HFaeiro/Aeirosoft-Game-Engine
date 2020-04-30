#pragma once
#include "framework.h"
#include <DirectXCollision.h>
#include "Vertex.h"
class aeBounding
{
public:
	aeBounding(){}
	operator DirectX::BoundingOrientedBox() const { return tOB; }
//	operator DirectX::BoundingOrientedBox (){ return tOB; }
	operator DirectX::BoundingSphere () const { return bSphere; }
	operator DirectX::XMMATRIX() const { return world; }
	const aeBounding& operator = (const DirectX::BoundingOrientedBox& b);
	const aeBounding& operator = (const DirectX::BoundingBox& b);
	bool Create(const std::vector<Vertex>& _vertices);
	bool CreateFromFloat3(DirectX::XMFLOAT3& size)
	{
		DirectX::XMFLOAT3 minVertex = { .5f * -size.x, 0, .5f * -size.z };
		DirectX::XMFLOAT3 maxVertex = { .5f * size.x, size.y, .5f * size.z };
		return CreateFromMinMax(minVertex, maxVertex);
	}
	bool CreateFromMinMax(std::vector<Vertex>& _vertices)
	{
		vertices = _vertices;
		DirectX::XMFLOAT3 minVertex = DirectX::XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
		DirectX::XMFLOAT3 maxVertex = DirectX::XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
		for (Vertex i : vertices)
		{
			minVertex.x = std::min(minVertex.x, i.position.x);
			minVertex.y = std::min(minVertex.y, i.position.y);
			minVertex.z = std::min(minVertex.z, i.position.z);

			maxVertex.x = std::max(maxVertex.x, i.position.x);
			maxVertex.y = std::max(maxVertex.y, i.position.y);
			maxVertex.z = std::max(maxVertex.z, i.position.z);

		}
		return CreateFromMinMax(minVertex, maxVertex);
	}
	bool CreateFromMinMax(DirectX::XMFLOAT3& min, DirectX::XMFLOAT3& max)
	{
		DirectX::XMFLOAT3 centerOffset = { .5f * (min.x + max.x), .5f * (min.y + max.y) , .5f * (min.z + max.z) };
		DirectX::XMFLOAT3 Extents = { .5f * (min.x - max.x), .5f * (min.y - max.y) , .5f * (min.z - max.z) };

		ogOB = DirectX::BoundingOrientedBox(centerOffset, Extents, { 0,0,0,1.f });
		//this might come to bite me in the arse?
		if (ogOB.Extents.z == Extents.z)
			return true;
		return false;
	}
	bool CreateFromPoints(std::vector<Vertex>& _vertices)
	{
		vertices = _vertices;
		std::vector<DirectX::XMFLOAT3> corners;
		bool add = true;
		for (const auto& i : vertices)
		{
			add = true;
			for (const auto& c : corners)
			{
				if (c.x == i.position.x && c.y == i.position.y && c.z == i.position.z)
				{
					add = false;
					break;
				}
			}
			if (add)
			{
				corners.push_back(i.position);
			}
		}
		if (corners.size() == 8)
		{
			ogOB.CreateFromPoints(ogOB, 8, corners.data(), sizeof(DirectX::XMFLOAT3));
			return true;
		}
		else
			return false;
	}
	void Transform(DirectX::XMMATRIX _world, bool makeSphere);
	void InverseTransformFromFloat3(DirectX::XMFLOAT3 f3);
	DirectX::BoundingSphere getSphere()const { return bSphere; }
	void setTransformPtr(DirectX::XMMATRIX* _transform) {
		transformation = _transform; 
	}
	std::vector<Vertex> getVertices() const { return vertices; }

	bool Resolve(const aeBounding& otherObject);

	//std::vector< int >QuadID;
	bool hasSphere = false;
	std::vector<DirectX::XMFLOAT3> vAdjustments;
private:

	DirectX::BoundingOrientedBox ogOB;
	DirectX::BoundingOrientedBox tOB;
	DirectX::BoundingBox ogBB;
	DirectX::BoundingSphere bSphere;
	std::vector<Vertex> vertices;
	DirectX::XMMATRIX* transformation = nullptr;
	DirectX::XMMATRIX world;

};

