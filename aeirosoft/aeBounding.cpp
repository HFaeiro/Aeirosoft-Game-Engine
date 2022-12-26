#include "aeBounding.h"

const aeBounding& aeBounding::operator=(const DirectX::BoundingOrientedBox& b)
{
	this->tOB = b;
	return *this;
}
const aeBounding& aeBounding::operator=(const DirectX::BoundingBox& b)
{
	this->ogBB = b;
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
	DirectX::XMVECTOR vCorners[8];
	DirectX::XMVECTOR vCornersT[8];
	corners.clear();
	corners.resize(8);

	if (useBB)
		ogBB.GetCorners(corners.data());
	else
		ogOB.GetCorners(corners.data());
	for (int i = 0; i < 8; i++)
	{
		vCorners[i] = { corners[i].x, corners[i].y, corners[i].z, 1.f };
		vCorners[i] = DirectX::XMVector3Transform(vCorners[i], transformation == nullptr ? world : DirectX::XMMatrixTranspose(*transformation) * world);
		DirectX::XMStoreFloat3(&corners[i], vCorners[i]);
	}
	
	if (useBB)
	{
		ogBB.CreateFromPoints(tBB, 8, corners.data(), sizeof(DirectX::XMFLOAT3));
		Extentsf3 = tBB.Extents;
		Center = tBB.Center;
	}
	else
	{

		ogOB.CreateFromPoints(tOB, 8, corners.data(), sizeof(DirectX::XMFLOAT3));
		Extentsf3 = tOB.Extents;
		Center = tOB.Center;
	}


	if (std::isnan(Extentsf3.z)) {
		tOB = ogOB;
	}
	float extents = Extentsf3.z;
	if (tOB.Extents.x > extents)
		extents = Extentsf3.x;
	if (tOB.Extents.y > extents)
		extents = Extentsf3.y;
	if (makeSphere)
		bSphere = DirectX::BoundingSphere(tOB.Center, extents);
	hasSphere = makeSphere;

}

void aeBounding::InverseTransformFromFloat3(DirectX::XMFLOAT3 f3)
{
	DirectX::XMMATRIX f3PosMatrix = DirectX::XMMatrixTranslation(f3.x, f3.y, f3.z);

	DirectX::XMMATRIX worldInverse = DirectX::XMMatrixInverse(nullptr, world);

	world = f3PosMatrix * worldInverse;
	Transform(world, true);



}

bool aeBounding::Resolve(const aeBounding& otherObject)
{
	using namespace DirectX;
	DirectX::XMFLOAT3 adjustment = { 0,0,0 };
	//Sphere vs OBB
	if (hasSphere && !otherObject.hasSphere) {
		/*DirectX::BoundingOrientedBox otherBox = otherObject.tOB;*/
		DirectX::XMFLOAT3 d = { bSphere.Center.x - otherObject.Center.x ,
			bSphere.Center.y - otherObject.Center.y  ,
			bSphere.Center.z - otherObject.Center.z };
		DirectX::XMVECTOR vD = DirectX::XMLoadFloat3(&d);
		DirectX::XMFLOAT3 closest;
		closest.x = std::clamp(d.x, -otherObject.Extentsf3.x, otherObject.Extentsf3.x);
		closest.y = std::clamp(d.y, -otherObject.Extentsf3.y, otherObject.Extentsf3.y);
		closest.z = std::clamp(d.z, -otherObject.Extentsf3.z, otherObject.Extentsf3.z);
		bool inside = false;

		if (d.x == closest.x && d.y == closest.y && d.z == closest.z)
		{
			inside = true;
			DirectX::XMFLOAT3 dAbs;
			DirectX::XMStoreFloat3(&dAbs, DirectX::XMVectorAbs(vD));

			////////////How do I include the 3rd dimension?////////////////////////////////////////
			if (dAbs.x > dAbs.z && dAbs.x > dAbs.y)
				if (closest.x > 0)
					closest.x = otherObject.Extentsf3.x;
				else
					closest.x = -otherObject.Extentsf3.x;
			else if (dAbs.z > dAbs.x && dAbs.z > dAbs.y)
				if (closest.z > 0)
					closest.z = otherObject.Extentsf3.z;
				else
					closest.z = -otherObject.Extentsf3.z;
			//////////////////////////// ???????????????????? guessing here
			else
				if (closest.y > 0)
					closest.y = otherObject.Extentsf3.y;
				else
					closest.y = -otherObject.Extentsf3.y;
			////////////////////////////////////////////////////////////////////////////////
		}

		DirectX::XMFLOAT3 normal = { d.x - closest.x , d.y - closest.y , d.z - closest.z };
		DirectX::XMVECTOR vNormal = DirectX::XMLoadFloat3(&normal);

		float normalMag = normal.x * normal.x + normal.y * normal.y + normal.z * normal.z;
		float radius = bSphere.Radius;

		if (normalMag > radius * radius)
		{
			return false;
		}

		normalMag = std::sqrt(normalMag);
		float penetration = radius - normalMag;
		float offset = penetration / (normalMag ? normalMag : 1);


		DirectX::XMStoreFloat3(&adjustment, (inside ? -vNormal : vNormal) * offset);
		if (!inside)
		{
			
			vAdjustments.push_back({ otherObject.aeID, adjustment });
		}
	}
	////Sphere vs Sphere
	else if (hasSphere && otherObject.hasSphere)
	{
		DirectX::BoundingSphere otherSphere = otherObject.bSphere;
		DirectX::XMFLOAT3 d = { bSphere.Center.x - otherSphere.Center.x ,
			bSphere.Center.y - otherSphere.Center.y  ,
			bSphere.Center.z - otherSphere.Center.z };

		float r = bSphere.Radius + otherSphere.Radius;
		float r2 = r * r;

		float normalMag = d.x * d.x + d.y * d.y + d.z * d.z;
		if (normalMag > r2)
			return false;

		normalMag = std::sqrt(normalMag);
		float penetration = 0;
		if (normalMag != 0)
		{
			penetration = (r - normalMag) * .5f;
		}
		else
			penetration = bSphere.Radius;


		DirectX::XMVECTOR dNormalized = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&d));
		DirectX::XMStoreFloat3(&adjustment, dNormalized * penetration);
		adjustment.y = 0;


		vAdjustments.push_back({ otherObject.aeID, adjustment });

	}
	return true;
}
