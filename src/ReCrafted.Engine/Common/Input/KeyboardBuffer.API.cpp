// ReCrafted © 2016-2017 Always Too Late

#include "KeyboardBuffer.h"

void KeyboardBuffer::initRuntime()
{
	API_FILE("Common/KeyboardBuffer.Gen.cs");
	{
		API_COMMENT("KeyboardBuffer class.");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "KeyboardBuffer", "Object", PARTIAL, NOCONSTRUCTOR);
		{
		}
		API_CLASS_END();
	}
	API_FILE_END();
}
