#pragma once
#include "framework.h"
#include <DirectXCollision.h>

class camera
{
public:

	enum class movementType { forward, backward, up, left, right };

	camera();
	~camera();

	void adjustPosition(movementType,float);
	//void adjustRotation(movementType, float, float, float);
	void setPosition(float, float, float);
	void setRotation(float, float, float);
	void setProjection(DirectX::XMMATRIX camProjection);
	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRoation();

	void render();
	DirectX::XMMATRIX getViewMatrix();


	DirectX::BoundingFrustum bFrustum;

private:
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 rot;
	DirectX::XMMATRIX viewMatrix;

	DirectX::XMVECTOR DefaultForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMVECTOR DefaultRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR DefaultBackward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
	DirectX::XMVECTOR DefaultLeft = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR DefaultUp = DirectX::XMVectorSet(0.0f, 1.0f, .0f, 0.0f);



	DirectX::XMVECTOR camForward;
	DirectX::XMVECTOR camBackward;
	DirectX::XMVECTOR camLeft;
	DirectX::XMVECTOR camRight;
	DirectX::XMVECTOR camUp;
	DirectX::XMMATRIX camProjection;

	DirectX::XMVECTOR vPos;


};

