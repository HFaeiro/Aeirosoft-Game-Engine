#pragma once
#include "framework.h"
#include <bitset>
#include <queue>
#include <memory>
class keyboard
{
public:
	class event
	{
	public:
		enum class Type
		{
			press,
			release,
			invalid
		};

	private:
		Type type;
		UCHAR code;
	public:
		event() :
			type(Type::invalid), code(0u)
		{}
		event(Type type, UCHAR code) :
			type(type), code(code)
		{}
		UCHAR getKey() { return code; }
	};

	keyboard();
	~keyboard();

	void keyDown(UCHAR keyCode);
	void keyUp(UCHAR keyCode);
	bool isKey(UCHAR keyCode);
	bool isEmpty()				{ return keyBuffer.empty(); }
	void flushKey()				{ keyBuffer = std::queue<event>(); }
	event readKey();
	

private:
	static constexpr INT16 nKeys = 256u;
	
	std::bitset<nKeys> keyStates;
	std::queue<keyboard::event> keyBuffer;
	std::queue<char> charBuffer;





};

