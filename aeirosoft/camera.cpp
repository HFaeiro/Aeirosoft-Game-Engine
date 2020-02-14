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
void camera::adjustPosition(movementType Type, float amount)
{
	prevPos = pos;
	switch (Type)
	{
	case movementType::forwardRight:
	{
		vPos += (camForward + camRight) * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;

	}
	case movementType::forwardLeft:
		vPos += (camForward + camLeft) * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;
	case movementType::backRight:
		vPos += (camBackward + camRight) * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;
	case movementType::backLeft:
		vPos += (camBackward + camLeft) * amount;
		DirectX::XMStoreFloat3(&pos, vPos);
		break;
		break;
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
	render();
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
void camera::adjustPosition(DirectX::XMFLOAT3 xmfloat)
{
	prevPos = pos;
	pos.x += xmfloat.x;
	pos.y += xmfloat.y;
	pos.z += xmfloat.z;
	vPos = XMLoadFloat3(&this->pos);
	render();
}
void camera::setPosition(DirectX::XMFLOAT3 xmfloat)
{
	pos.x = xmfloat.x;
	pos.y = xmfloat.y;
	pos.z = xmfloat.z;
	vPos = XMLoadFloat3(&this->pos);
	render();

}
void camera::setPosition(float x, float y, float z)
{

	pos.x = x;
	pos.y = y;
	pos.z = z;
	vPos = XMLoadFloat3(&this->pos);
	render();

}

void camera::setRotation(float x, float y, float z)
{
	rot.x = x;
	rot.y = y;
	rot.z = z;
	render();
}

void camera::setProjection(XMMATRIX camProjection)
{
	this->camProjection = camProjection;
}

DirectX::XMFLOAT3 camera::getPosition()
{
	return pos;
}

DirectX::XMFLOAT3 camera::getRotation()
{
	return rot;
}

void camera::render(bool spectate)
{
	XMVECTOR camPosition;
	XMVECTOR camTarget;
	XMMATRIX camView;

	DirectX::XMMATRIX rotationMatrix;

	camPosition = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);


	rotationMatrix = XMMatrixRotationRollPitchYaw(rot.x, rot.y, 0.f);
	camTarget = XMVector3TransformCoord(DefaultForward, rotationMatrix);
	camTarget = XMVector3Normalize(camTarget);


	if (!spectate)
	{
		XMMATRIX RotateYTempMatrix;
		RotateYTempMatrix = XMMatrixRotationY(rot.y);

		camRight = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
		camUp = XMVector3TransformCoord(DefaultUp, RotateYTempMatrix);
		camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);
		camBackward = XMVector3TransformCoord(DefaultBackward, RotateYTempMatrix);
		camLeft = XMVector3TransformCoord(DefaultLeft, RotateYTempMatrix);
	}
	else 
	{
		camRight = XMVector3TransformCoord(DefaultRight, rotationMatrix);
		camUp = XMVector3TransformCoord(DefaultUp, rotationMatrix);
		camForward = XMVector3TransformCoord(DefaultForward, rotationMatrix);
		camBackward = XMVector3TransformCoord(DefaultBackward, rotationMatrix);
		camLeft = XMVector3TransformCoord(DefaultLeft, rotationMatrix);
	}
	camTarget += camPosition;

	prevView = viewMatrix;
	viewMatrix = XMMatrixLookAtLH(camPosition, camTarget, camUp);


}

DirectX::XMMATRIX camera::getViewMatrix()
{
	return viewMatrix;
}
