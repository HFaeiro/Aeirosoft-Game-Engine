#pragma once
#include "Collision.h"

#include "input.h"
#include "model.h"
#include <string>

class Entity : public Collidable, 
	 public camera, private model
{
public:
	Entity(graphics* g, input* _i, std::wstring filename, float scale = 1.f) : model(filename, g, scale), Collidable(g), camera(g->m_Camera), i(_i)
	{
		Collidable::type = Collidable::Entity;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			CreateBoundingOrientedBox(v);
		}

	}
	

	~Entity() {}
	input* i;
	const std::wstring getName() const { return name; }

private:
	std::wstring name;


};
class EntityAi : public Collidable, public model
{
public:
	EntityAi(graphics* g, std::wstring filename, float scale = 1.f) : model(filename, g, scale), Collidable(g)
	{
		Collidable::type = Collidable::EntityAi;
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			CreateBoundingOrientedBox(v);
		}
	}
	~EntityAi() {}

	const std::wstring getName() const { return name; }

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
	}
	~EntityObject() {}

	const std::wstring getName() const { return name; }

private:
	std::wstring name;

};