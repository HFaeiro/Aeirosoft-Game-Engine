#include "aeBounding.h"

const aeBounding& aeBounding::operator=(const DirectX::BoundingOrientedBox& b)
{
	this->tOB = b;
	return *this;
}

bool aeBounding::Create(const std::vector<Vertex>& _vertices)
{
	vertices = _vertices;
	DWORD vsize = vertices.size();
	if (!vsize)
		return false;
	if (vsize == 4)
		return false;
	if (vsize == 24 ||/* vsize == 24 || vsize == 20 || vsize == 48 ||*/ vsize == 30)
	{
		return CreateFromPoints(vertices);
	}
	else {
		return CreateFromMinMax(vertices);
	}
}

void aeBounding::Transform(DirectX::XMMATRIX _world, bool makeSphere)
{
	world = _world;
	DirectX::XMFLOAT3 corners[8];
	DirectX::XMVECTOR vCorners[8];
	ogOB.GetCorners(corners);
	for (int i = 0; i < 8; i++)
	{
		vCorners[i] = { corners[i].x, corners[i].y, corners[i].z, 1.f };
		vCorners[i] = DirectX::XMVector3Transform(vCorners[i], transformation == nullptr ? world : DirectX::XMMatrixTranspose(*transformation) * world);
		DirectX::XMStoreFloat3(&corners[i], vCorners[i]);

	}
	//ogOB.Transform(tOB, transformation == nullptr ? world : DirectX::XMMatrixTranspose(*transformation) * world);
	//
	//float extents = tOB.Extents.z;
	////if (tOB.Extents.x > extents)
	////	extents = tOB.Extents.x;
	////if (tOB.Extents.y > extents)
	////	extents = tOB.Extents.y;
	ogOB.CreateFromPoints(tOB, 8, corners, sizeof(DirectX::XMFLOAT3));
	float extents = tOB.Extents.z;
	if (std::isnan(tOB.Extents.z)) {
		tOB = ogOB;
	}
	if (!std::isnan(extents) && makeSphere)
		bSphere = DirectX::BoundingSphere(tOB.Center, extents);
}
