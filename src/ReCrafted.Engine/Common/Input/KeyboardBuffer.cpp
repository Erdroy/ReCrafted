// ReCrafted © 2016-2017 Always Too Late

#include "KeyboardBuffer.h"

Ptr<KeyboardBuffer> KeyboardBuffer::m_instance;

void KeyboardBuffer::init()
{
	m_instance = Object::createInstance<KeyboardBuffer>("ReCrafted.API.Common", "KeyboardBuffer");
	m_instance->m_buffer.reserve(128);
}

void KeyboardBuffer::shutdown()
{
	Object::destroy(m_instance);
}

void KeyboardBuffer::clear()
{
	m_instance->m_buffer.clear();
}

void KeyboardBuffer::write(Char character)
{
	m_instance->m_buffer.add(character); // TODO: use lParam?
}

Array<Char>* KeyboardBuffer::getBuffer()
{
	return &m_instance->m_buffer;
}
