#include "keyboard.h"

keyboard::keyboard()
{
}

keyboard::~keyboard()
{
}

void keyboard::keyDown(UCHAR keyCode)
{
    keyStates[keyCode] = true;
    keyBuffer.push(event(event::event::Type::press, keyCode));
}

void keyboard::keyUp(UCHAR keyCode)
{
    keyStates[keyCode] = false;
    keyBuffer.push(event(event::Type::release, keyCode));
    
}

bool keyboard::isKey(UCHAR keyCode)
{
    return keyStates[keyCode];
}
