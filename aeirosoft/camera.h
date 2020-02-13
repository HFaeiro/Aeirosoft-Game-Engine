#pragma once
#include "framework.h"
#include <DirectXCollision.h>

class camera
{
public:

	enum class movementType { forward, backward, up, left, right, forwardRight, forwardLeft, backRight, backLeft};

	camera();
	~camera();

	virtual void adjustPosition(movementType, float);
	virtual void setPosition(DirectX::XMFLOAT3 xmfloat);
	//void adjustRotation(movementType, float, float, float);
	virtual void setPosition(float, float, float);
	virtual void setRotation(float, float, float);
	void setProjection(DirectX::XMMATRIX camProjection);
	virtual DirectX::XMFLOAT3 getPosition();
	virtual DirectX::XMFLOAT3 getRotation();
	void revertView()
	{
		
		pos = prevPos;
		render();

	}

	void render(bool spectate = false);
	virtual DirectX::XMMATRIX getViewMatrix();



	/*DirectX::BoundingFrustum bFrustum;*/

private:

	DirectX::XMFLOAT3 pos = {};
	DirectX::XMFLOAT3 prevPos = {};
	DirectX::XMFLOAT3 rot = {};
	DirectX::XMMATRIX viewMatrix = {};
	DirectX::XMMATRIX prevView = {};;
	//DirectX::XMMATRIX worldMatrix;

	DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR DefaultForwardRight = DirectX::XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR DefaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR DefaultBackward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	DirectX::XMVECTOR DefaultLeft = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR DefaultUp = DirectX::XMVectorSet(0.0f, 1.0f, .0f, 0.0f);



	DirectX::XMVECTOR camForward = {};
	DirectX::XMVECTOR camForwardRight;
	DirectX::XMVECTOR camBackward;
	DirectX::XMVECTOR camLeft = {};
	DirectX::XMVECTOR camRight = {};
	DirectX::XMVECTOR camUp = {};
	DirectX::XMMATRIX camProjection = {};

	DirectX::XMVECTOR vPos;


};

