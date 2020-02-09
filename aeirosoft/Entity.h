#pragma once
#include "Collision.h"
#include "model.h"
#include <string>
class Entity : public Collidable, public model
{
public:
	Entity(graphics *g, std::wstring filename, float scale = 1.f): model(filename, g, scale), Collidable(g) 
	{
		name = helper::strings::GetNameFromPath(filename);
		std::vector< std::vector<Vertex>> vertices = getVertices();
		for (const auto& v : vertices)
		{
			CreateBoundingOrientedBox(v);
		}
	}
	~Entity() {}

	const std::wstring getName() const { return name; }

private:
	std::wstring name;

};

