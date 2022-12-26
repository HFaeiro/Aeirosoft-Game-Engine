#pragma once
#include "framework.h"
class interactable
{
public:
	interactable() {}
	~interactable() {}

	virtual void interact() = 0;

};

