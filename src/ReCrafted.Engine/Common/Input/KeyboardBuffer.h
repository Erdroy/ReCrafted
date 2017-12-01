// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef KEYBOARDBUFFER_H
#define KEYBOARDBUFFER_H

// includes
#include "ReCrafted.h"
#include "Core/Containers/Array.h"
#include "Common/Text.h"
#include "Scripting/Object.h"

class KeyboardBuffer : public Object
{
	API_DEF

private:
	static Ptr<KeyboardBuffer> m_instance;

private:
	Array<Char> m_buffer = {};

public:
	static void init();
	static void shutdown();

	static void clear();
	static void write(Char character);
};

#endif // KEYBOARDBUFFER_H
