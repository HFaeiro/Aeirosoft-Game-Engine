#pragma once
#include "framework.h"
#include "Item.h"
class Container : public Item
{
public:
	Container(Item item, int size = 1) : Item(item), containerSize(size) {
		items = new int[containerSize];
		memset(items, 0, containerSize * sizeof(int));
	};
	~Container() {
		if (items != nullptr)
			delete items;
	};

	bool isFull()
	{
		return containerSize == numItems;
	}
	bool AddItem(int itemId)
	{
		if(containerSize != numItems)
		if (items[numItems] == NULL)
		{
			items[numItems] = itemId;
			numItems++;
			return true;
		}
		return false;
	}
	int numItems = 0;
	int containerSize = 1;
	int* items;



};

