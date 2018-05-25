// ReCrafted (c) 2016-2018 Always Too Late

#include "KeyboardBuffer.h"

Array<Char> KeyboardBuffer::m_buffer;

void KeyboardBuffer::Clear()
{
    m_buffer.Clear();
}

void KeyboardBuffer::Write(Char character)
{
    m_buffer.Add(character); // TODO: use lParam?
}

Array<Char>* KeyboardBuffer::GetBuffer()
{
    return &m_buffer;
}
