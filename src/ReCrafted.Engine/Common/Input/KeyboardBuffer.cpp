// ReCrafted (c) 2016-2018 Always Too Late

#include "KeyboardBuffer.h"

Array<Char> KeyboardBuffer::m_buffer;

void KeyboardBuffer::clear()
{
	m_buffer.clear();
}

void KeyboardBuffer::write(Char character)
{
	m_buffer.add(character); // TODO: use lParam?
}

Array<Char>* KeyboardBuffer::getBuffer()
{
	return &m_buffer;
}
