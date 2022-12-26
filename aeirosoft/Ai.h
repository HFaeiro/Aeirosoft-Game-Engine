#pragma once
#include "framework.h"
#include "Entity.h"
class Ai :public Entity
	{
	public:

		//virtual bool Initialize() = 0;
		//virtual void Update() = 0;
		//virtual std::optional<Events*> Queue() = 0;
		Ai(const Ai& e) : Entity(e)
		{
			this->name = e.name;
			std::vector< std::vector<Vertex>> vertices = getVertices();
			std::vector<DirectX::XMMATRIX*> transformations = getTransforms();
			CreateBoundingOrientedBox(vertices, transformations);
		}
		Ai(graphics* g, std::wstring filename, float scale = 1.f) : Entity(g, filename, scale)
		{
			Collidable::type = Collidable::EntityAi;
			name = helper::strings::GetNameFromPath(filename);
			std::vector< std::vector<Vertex>> vertices = getVertices();
			std::vector<DirectX::XMMATRIX*> transformations = getTransforms();
			CreateBoundingOrientedBox(vertices, transformations);
		}
		~Ai()
		{
			Entity::~Entity();
		}
		void Update() override
		{
			for (const auto& adjustment : Boundings[0].vAdjustments)
			{
				adjustPosition(adjustment.resolve);
			}

			TransformBounds(getWorld());
			Boundings[0].vAdjustments.clear();
		}
		const std::wstring getName() const { return name; }
		//void _Update();

	protected:
		std::wstring name;


};

