#include "Controllable.h"

bool Controllable::Initialize()
{
	return false;
}

void Controllable::Update()
{
	std::vector<int> CollidedWithIds;

	for (auto& adjustment : Boundings[0].vAdjustments)
	{
		bool pushId = true;
		for (const auto& id : CollidedWithIds)
		{
			if (adjustment.collidedID == id)
			{
				pushId = false;
				continue;
			}
		}
		if (pushId) {
			CollidedWithIds.push_back(adjustment.collidedID);
			if (adjustment.resolve.y > 0)
				falling = false;
			else if (adjustment.resolve.y < 0)
				falling = true;
			adjustPosition(adjustment.resolve);
		}

	}
	Boundings[0].vAdjustments.clear();
	TransformBounds(getWorldAtViewMatrix());
}

std::optional<Events*> Controllable::Queue()
{
	return std::optional<Events*>();
}

DirectX::XMFLOAT3 Controllable::getPosition()
{
	
	return _g->m_Camera.getPosition();
}

DirectX::XMFLOAT3 Controllable::getRotation()
{
	return _g->m_Camera.getRotation();
}

void Controllable::setPosition(DirectX::XMFLOAT3 xmfloat)
{
	entityHeight = xmfloat.y + 2;  _g->m_Camera.setPosition(xmfloat);
}

void Controllable::setPosition(float x, float y, float z)
{
	_g->m_Camera.setPosition(x, y, z);
	TransformBounds(getWorldAtViewMatrix());
}

void Controllable::adjustPosition(DirectX::XMFLOAT3 pos)
{
	_g->m_Camera.adjustPosition(pos);
}

void Controllable::adjustPosition(camera::movementType type, float velocity)
{
	if (velocity > 5 && velocity < -5)
		if (velocity > 5)
			velocity = 0;
		else
			velocity = 0;
	prevMove = type;
	recentVelocity = velocity;

	_g->m_Camera.adjustPosition(type, velocity);
	TransformBounds(getWorldAtViewMatrix());
	return;
}

bool Controllable::isLeftClick() { return i->isLeftClick(); }

bool Controllable::isRightClick() { return i->isRightClick(); }

bool Controllable::isKey(UCHAR c) {
	return i->isKey(c);
}

DirectX::XMMATRIX Controllable::getWorldAtViewMatrix()
{
	DirectX::XMFLOAT3 rot = _g->m_Camera.getRotation();
	DirectX::XMFLOAT3 pos = _g->m_Camera.getPosition();
	return  DirectX::XMMatrixRotationRollPitchYaw(0, rot.y, 0) * DirectX::XMMatrixTranslation(pos.x, pos.y - entityHeight, pos.z);
}

DirectX::XMMATRIX Controllable::getViewMatrix() { return _g->GetViewMatrix(); }
