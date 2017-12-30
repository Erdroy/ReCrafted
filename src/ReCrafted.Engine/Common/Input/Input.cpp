// ReCrafted © 2016-2017 Always Too Late

#include "Input.h"

Input* Input::m_instance;

void Input::init()
{
	// clear all keys
	releaseAll();
}

void Input::emit(bool up, uint key)
{
	m_keys[key] = up ? 0 : 1;
}

void Input::emitScroll(float delta)
{
    m_scrollDelta_u += delta;
}

void Input::update(int cursorX, int cursorY, int deltaX, int deltaY)
{
	// copy all keys
	for (auto i = 0; i < INPUT_TABLE_SIZE; i++)
		m_lastkeys[i] = m_keys[i];

	// update cursor pos
	m_cursorPos = Vector2(float(cursorX), float(cursorY));
	m_cursorDelta = Vector2(float(deltaX), float(deltaY));

    // clear scroll
    m_scrollDelta = m_scrollDelta_u;
    m_scrollDelta_u = 0.0f;
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

void Input::dispose()
{
	// clear all keys
	releaseAll();

	// suicide
	delete this;
}