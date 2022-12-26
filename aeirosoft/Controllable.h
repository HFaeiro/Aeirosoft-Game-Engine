#pragma once
#include "framework.h"
#include "Entity.h"
#include "input.h"
class Controllable : public Entity
{
public:
	Controllable(graphics* g, input* i, std::wstring filename) : Entity(g, filename), i(i), _g(g)
	{}
	~Controllable()
	{
		Entity::~Entity();
	}
protected:
	input* i;
	graphics* _g;

	// Inherited via Entity
	virtual bool Initialize() override;
	virtual void Update() override;
	virtual std::optional<Events*> Queue() override;
	DirectX::XMFLOAT3 getPosition();
	DirectX::XMFLOAT3 getRotation();
	virtual void setPosition(DirectX::XMFLOAT3 xmfloat) override;
	virtual void setPosition(float x, float y, float z) override;
	virtual void adjustPosition(DirectX::XMFLOAT3 pos) override;
	void adjustPosition(camera::movementType type, float velocity);
	void CreateBoundingOrientedBox(DirectX::XMFLOAT3 size)
	{
		entityHeight = size.y;
		Collidable::CreateBoundingOrientedBox(size);
	}
	bool isLeftClick();;
	bool isRightClick();;
	bool isKey(UCHAR c);
	float entityHeight;
	DirectX::XMMATRIX getWorldAtViewMatrix();
	DirectX::XMMATRIX getViewMatrix();
};

