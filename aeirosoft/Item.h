#pragma once
#include "framework.h"
#include "model.h"
#include "Entity.h"
#include "interactable.h"
enum ItemStatus
{
	Ground, Equiped, Inventory
};
struct ItemStruct
{
	int id;
	int equipable;
	int maxStack;
	int count;
	std::string name;
	std::wstring grndModFile, eqptModFile, invModFile;
	ItemStatus status;
};
class Item : public Entity, public interactable
{
public:
	Item(graphics* g, ItemStruct iStruct,  float scale = 1.f)
		:g(g), Entity(g, iStruct.grndModFile, scale), itemInfo(iStruct)
	{
		if (iStruct.equipable)
		{
			equipModel = new model(iStruct.eqptModFile, g, scale);
		}
	}
	~Item() {}

	Item(const Item& i) : Entity(i)
	{

		*this = i;

	}
	ItemStruct itemInfo;
	virtual void UpdateWorldMatrixWithViewMatrix(DirectX::XMMATRIX vMatrix) 
	{
		if (itemInfo.status != ItemStatus::Ground)
		{
			equipModel->UpdateWorldMatrixWithViewMatrix(vMatrix);
		}
		else
			model::UpdateWorldMatrixWithViewMatrix(vMatrix);
	}
	virtual void Update() override
	{
		if (itemInfo.status != ItemStatus::Ground)
		{
			equipModel->Render(g->m_TextureShader);
		}
		else
			Entity::Update();
	}
	virtual void SetCurrentAnimation(std::string animName)
	{
		if (itemInfo.status != ItemStatus::Ground)
		{
			equipModel->SetCurrentAnimation(animName);
		}
		else
			model::SetCurrentAnimation(animName);
	}
	virtual bool LeftClick()
	{
		return false;
	}
	// Inherited via interactable
	virtual void interact() override;


	// Inherited via Entity
	virtual bool Initialize() override;

	virtual std::optional<void*> OnCollision() override
	{
		return this;
	}
private:
	graphics* g;
	model* equipModel;
};

