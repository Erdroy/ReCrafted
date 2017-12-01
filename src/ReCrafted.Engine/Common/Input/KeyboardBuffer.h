// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef KEYBOARDBUFFER_H
#define KEYBOARDBUFFER_H

// includes
#include "Common/Text.h"
#include "Core/Containers/Array.h"
#include "Scripting/Object.h"

class KeyboardBuffer
{
	API_DEF

private:
	static Array<Char> m_buffer;

public:
	static void clear();
	static void write(Char character);

	static Array<Char>* getBuffer();
};

#endif // KEYBOARDBUFFER_H
