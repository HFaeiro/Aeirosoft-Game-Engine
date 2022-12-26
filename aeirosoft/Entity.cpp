#include "Entity.h"

bool Entity::Initialize()
{

	return true;
}

void Entity::Update()
{
	//TransformBounds(getWorld());
	Render(_g->m_TextureShader);
	return;
}

std::optional<Events*> Entity::Queue()
{
	if (Q)
		return this;
	else
		return std::optional<Events*>();
}
