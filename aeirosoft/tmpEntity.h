#pragma once
#include "Collision.h"
#include "input.h"
#include "model.h"
#include <string>
class EntityAi :public Events, public Collidable, public model
{
public:
	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual std::optional<Events*> Queue() = 0;
	EntityAi(const EntityAi& e) : model(e), Collidable(e)
	{
		this->name = e.name;
		std::vector< std::vector<Vertex>> vertices = getVertices();
		std::vector<DirectX::XMMATRIX*> transformations = getTransforms();
		//for (const auto& v : vertices)
		//{
		CreateBoundingOrientedBox(vertices, transformations);
	}
	EntityAi(graphics* g, std::wstring filename, float scale = 1.f) : model(filename, g, scale), Collidable(g)
	{
		Collidable::type = Collidable::EntityAi;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		std::vector<DirectX::XMMATRIX*> transformations = getTransforms();
		//for (const auto& v : vertices)
		//{
			CreateBoundingOrientedBox(vertices, transformations);
		//}
	}
	~EntityAi()
	{

		model::~model();
	}

	const std::wstring getName() const { return name; }
	void _Update();

private:
	std::wstring name;

};







class EntityObject : public Collidable, public model
{
public:
	EntityObject(graphics* g, std::wstring filename, float scale = 1.f) : model(filename, g, scale), Collidable(g)
	{
		Collidable::type = Collidable::Object;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			CreateBoundingOrientedBox(v);
		}
		//CreateBoundingOrientedBox(vertices);
	}
	EntityObject(const EntityObject& e) : model(e), Collidable(e)
	{ 

		*this = e;

	}
	~EntityObject() {
		/*((model*)this)->~model();*/
	}

	const std::wstring getName() const { return name; }

private:

	std::wstring name;

};