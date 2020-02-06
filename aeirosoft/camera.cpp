#include "camera.h"

using namespace DirectX;

camera::camera()
{
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->vPos = XMLoadFloat3(&this->pos);

	rot.x = 0;
	rot.y = 0;
	rot.z = 0;
}

camera::~camera()
{

}
void camera::adjustPosition(movementType Type,float amount)
{

	switch (Type)
	{
	case movementType::forward:
	{
		vPos += camForward * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;
	}
	case movementType::backward:
	{
		vPos += camBackward * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;
	}
	case movementType::left:
	{
		vPos += camLeft * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;
	}
	case movementType::right:
	{
		vPos += camRight * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;
	}
	case movementType::up:
	{
		vPos += camUp * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;
	}


	}

}
//void camera::adjustRotation(float, float, float)
//{
//}
DirectX::XMFLOAT3 operator+(DirectX::XMFLOAT3 l, DirectX::XMFLOAT3 r)
{
	DirectX::XMFLOAT3 n;
	n.x = l.x + r.x;
	n.y = l.y + r.y;
	n.z = l.z + r.z;
	return n;
}
void camera::setPosition(float x , float y , float z)
{

	pos.x = x;
	pos.y = y;
	pos.z = z;

	DirectX::XMStoreFloat3(&pos, vPos);

}

void camera::setRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
}

void camera::setProjection(XMMATRIX camProjection)
{
	this->camProjection = camProjection;
}

DirectX::XMFLOAT3 camera::getPosition()
{
	return pos;
}

DirectX::XMFLOAT3 camera::getRoation()
{
	return rot;
}

void camera::render()
{
	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMMATRIX WVP;
	XMMATRIX World;
	XMMATRIX camView;



	DirectX::XMMATRIX rotationMatrix;

	camPosition = XMVectorSet(pos.x, pos.y, pos.z, 0.0f);


	rotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.f);
	camTarget = XMVector3TransformCoord(DefaultForward, rotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(rot.y);

	camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp = XMVector3TransformCoord(DefaultUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
	camBackward = XMVector3TransformCoord(DefaultBackward, RotateYTempMatrix);
	camLeft = XMVector3TransformCoord(DefaultLeft, RotateYTempMatrix);

	////camPosition += xPos * camRight;
	////camPosition += yPos * camForward;

	////xPos = 0.0f;
	////yPos = 0.0f;

	camTarget += vPos;


	viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);
	XMVECTOR determinant;
	XMMATRIX invViewMatrix = XMMatrixInverse(&determinant, viewMatrix);
	
	bFrustum = BoundingFrustum();
	bFrustum.CreateFromMatrix(bFrustum, camProjection);
	bFrustum.Transform(bFrustum, invViewMatrix);


}

DirectX::XMMATRIX camera::getViewMatrix()
{
	 return viewMatrix;
}
