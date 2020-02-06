#pragma once
#include "framework.h"
#include <optional>



class Events
{
public:
	Events() {};
	virtual ~Events() {};

	virtual bool Initialize() = 0;
	virtual void Update() = 0;
	virtual std::optional<Events*> Queue() = 0;
	


};

//virtual bool Initialize()
//{
//	return true;
//}
//virtual void Update()
//{
//	return;
//}
//virtual std::optional<Events*> Queue()
//{
//	return {};
//}