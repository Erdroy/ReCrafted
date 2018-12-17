// ReCrafted (c) 2016-2018 Always Too Late

#include "Mouse.h"

void Mouse::Update()
{
}

bool Mouse::IsButton(Button button)
{
    return false;
}

bool Mouse::IsButtonDown(Button button)
{
    return false;
}

bool Mouse::IsButtonUp(Button button)
{
    return false;
}

void Mouse::SetMousePosition(const Vector2& position)
{
}

Vector2 Mouse::GetMousePosition()
{
    return Vector2::Zero;
}

Vector2 Mouse::GetMouseDelta()
{
    return Vector2::Zero;
}

int Mouse::GetMouseScrollDelta()
{
    return 0;
}

void Mouse::SetShowCursor(bool show)
{
}

bool Mouse::GetShowCursor()
{
    return false;
}

void Mouse::SetLockCursor(bool lock)
{
}

bool Mouse::GetLockCursor()
{
    return false;
}
