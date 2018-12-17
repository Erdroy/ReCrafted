// ReCrafted (c) 2016-2018 Always Too Late

#include "Keyboard.h"

void Keyboard::Update()
{
}

bool Keyboard::IsKey(Key key)
{
    return false;
}

bool Keyboard::IsKeyDown(Key key)
{
    return false;
}

bool Keyboard::IsKeyUp(Key key)
{
    return false;
}

Text&& Keyboard::GetInputString()
{
    return Text::Empty();
}
