#pragma once
#include "framework.h"
#include <sstream>
#include "Item.h"
#include "Container.h"

class Inventory
{
public:
	Inventory() {}
	Inventory(Item* item, int amount)
	{
		addItem(item, amount);
	}
	~Inventory() {}


	bool addItem(Item* item, int amount)
	{
		/*item->Q = false;
		item->Cthis->RemoveCollidable(item);*/
		/*if (item->maxStack > 1)
			for (auto& i : items)
			{
				if (i.id == item->id)
					i.count += amount;
			}
		else
			items.push_back(*item);*/
		bool retval = false;
		if (item->itemInfo.equipable)
		{

			switch (item->itemInfo.equipable - 1)
			{
			case 0:
				if (inventory.primary == nullptr) {
					inventory.primary = item;
					retval = true;
				}
				break;
			case 1:
				if (inventory.secondary == nullptr) {
					inventory.secondary = item;
					retval = true;
				}
				break;
			case 2:
				if (inventory.head == nullptr) {
					inventory.head = item;
					retval = true;
				}
				break;
			case 3:
				if (inventory.body == nullptr) {
					inventory.body = item;
					retval = true;
				}
				break;
			}
			if (retval)
				item->itemInfo.status = ItemStatus::Equiped;
			else
			{
				retval = inventory.backPack->AddItem(item->itemInfo.id);
				if(retval)
					item->itemInfo.status = ItemStatus::Inventory;
			}
		}
		else
			retval = inventory.backPack->AddItem(item->itemInfo.id);

		if (retval)
		{
			item->Q = false;
			item->Cthis->RemoveCollidable(item);
			if (item->itemInfo.maxStack > 1)
				for (auto& i : items)
				{
					if (i.itemInfo.id == item->itemInfo.id)
						i.itemInfo.count += amount;
				}
			else
				items.push_back(*item);

			item->itemInfo.status = ItemStatus::Equiped; ///need to change this to status inventory in future...
			item->setPositionAndRotation({ 0,0,0 }, { 0,0,0 });
		}
		return retval;
	}

	struct Character
	{
		Item* head;
		Item* body;
		Item* primary;
		Item* secondary;
		Container* backPack;
	};

	Character inventory;
	
	std::stringstream getItems()
	{
		std::stringstream ss;
		for (const auto& item : items)
		{
			ss << item.itemInfo.name << ": " << item.itemInfo.count << std::endl;
		}
		return ss;
	}
private:
	std::vector<Item> items;
};

