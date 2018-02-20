// ReCrafted (c) 2016-2018 Always Too Late

#include "Input.h"

Input* Input::m_instance;

void Input::onInit()
{
    // clear all keys
    releaseAll();
}

void Input::onShutdown()
{
    // clear all keys
    releaseAll();
}

void Input::update()
{
    // copy all keys
    for (auto i = 0; i < INPUT_TABLE_SIZE; i++)
        m_lastkeys[i] = m_keys[i];

    // update cursor pos
    m_cursorPos = Vector2(float(0), float(0)); // TODO: implement event-based cursor update
    m_cursorDelta = Vector2(float(0), float(0));

    // clear scroll
    m_scrollDelta = m_scrollDelta_u;
    m_scrollDelta_u = 0.0f;
}

void Input::emit(bool up, uint key)
{
	m_keys[key] = up ? 0 : 1;
}

void Input::emitScroll(float delta)
{
    m_scrollDelta_u += delta;
}

void Input::releaseAll()
{
	// clear all keys
	for (auto i = 0; i < INPUT_TABLE_SIZE; i++)
		m_keys[i] = byte(0);

    // clear scroll
    m_scrollDelta_u = 0.0f;
    m_scrollDelta = 0.0f;
}
