#pragma once
#include "framework.h"
#include <DirectXMath.h>

class moveable
{
public:
	moveable() {}
	~moveable() {}
	enum class movementType { forward, backward, up, left, right };
	virtual void adjustPosition(movementType, float) = 0;
	virtual void setPosition(DirectX::XMFLOAT3 xmfloat) = 0;
	virtual void setPosition(float, float, float) = 0;
	virtual void setRotation(float, float, float) = 0;
	virtual DirectX::XMFLOAT3 getPosition() = 0;
	virtual DirectX::XMFLOAT3 getRotation() = 0;




};

