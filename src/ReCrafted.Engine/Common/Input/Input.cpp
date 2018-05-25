// ReCrafted (c) 2016-2018 Always Too Late

#include "Input.h"
#include "Common/Display.h"

SINGLETON_IMPL(Input)

int m_cursorX = 0u;
int m_cursorY = 0u;
int m_cursorDeltaX = 0u;
int m_cursorDeltaY = 0u;

void Input::OnInit()
{
    // clear all keys
    ReleaseAllKeys();
}

void Input::OnDispose()
{
    // clear all keys
    ReleaseAllKeys();
}

void Input::UpdateInput()
{
    // copy all keys
    for (auto i = 0; i < INPUT_TABLE_SIZE; i++)
        m_lastkeys[i] = m_keys[i];

    // Update cursor pos
    m_cursorPos = Vector2(float(m_cursorX), float(m_cursorY)); // TODO: implement event-based cursor Update
    m_cursorDelta = Vector2(float(m_cursorDeltaX), float(m_cursorDeltaY));

    m_cursorDeltaX = 0;
    m_cursorDeltaY = 0;

    // clear scroll
    m_scrollDelta = m_scrollDelta_u;
    m_scrollDelta_u = 0.0f;

    // lock cursor
    if (m_lockCursor)
    {
        // lock position
        SetCursorPos(
            static_cast<int>(round(Display::GetWidth() / 2.0f)),
            static_cast<int>(round(Display::GetHeight() / 2.0f))
        );
    }
}

void Input::OnKeyEvent(bool up, uint key)
{
    m_keys[key] = up ? 0 : 1;
}

void Input::OnScrollEvent(float delta)
{
    m_scrollDelta_u += delta;
}

void Input::OnCursorEvent(int deltaX, int deltaY, int cursorX, int cursorY)
{
    m_cursorDeltaX += deltaX;
    m_cursorDeltaY += deltaY;

    m_cursorX = cursorX;
    m_cursorY = cursorY;
}

void Input::ReleaseAllKeys()
{
    // clear all keys
    for (auto i = 0; i < INPUT_TABLE_SIZE; i++)
        m_keys[i] = byte(0);

    // clear scroll
    m_scrollDelta_u = 0.0f;
    m_scrollDelta = 0.0f;
}
