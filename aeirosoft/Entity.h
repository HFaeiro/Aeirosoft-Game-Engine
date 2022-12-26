#pragma once
#include "Collision.h"

#include "model.h"
#include <string>


class Entity : public Events, public Collidable, 
	 public model
{
public:
	// Inherited via Events
	virtual bool Initialize() override;

	virtual void Update() override;

	virtual std::optional<Events*> Queue() override;


	Entity(graphics* g, std::wstring filename,float scale = 1.f) : model(filename, g, scale), _g(g), Collidable(g)
	{

		Collidable::type = Collidable::Object;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			Collidable::CreateBoundingOrientedBox(v);
		}
		//setPositionAndRotation(pos, rot);
		//TransformBounds(getWorld());
	}

	~Entity() 
	{
		((model*)this)->~model();
	}

	//virtual DirectX::XMFLOAT3 getPosition() = 0;
	//virtual DirectX::XMFLOAT3 getRotation() = 0;
	//virtual void setPosition(DirectX::XMFLOAT3 xmfloat) = 0;
	//virtual void adjustPosition(camera::movementType type, float velocity) = 0;

	camera::movementType prevMove;
	float recentVelocity;
	float prevVelocity;
	const std::wstring getName() const { return name; }
	bool falling = false;
	bool Q = true;
protected:

	//void CreateBoundingOrientedBox(std::vector<std::vector<Vertex>> v, std::vector<DirectX::XMMATRIX*> transforms)
	//{
	//	Collidable::CreateBoundingOrientedBox(v, transforms);
	//	Collidable::GetBounds();
	//}


	std::wstring name;
	graphics* _g;





};
