// ReCrafted (c) 2016-2018 Always Too Late

#include "KeyboardBuffer.h"
#include "ReCrafted.h"
#include "Common/ReCraftedAPI.h"
#include "Scripting/Mono.h"

namespace Internal
{
	MonoArray* read()
	{
		var buffer = KeyboardBuffer::getBuffer();

		var uintClass = mono_get_uint16_class();
		var array = mono_array_new(Domain::Root->getMono(), uintClass, buffer->count());

		// copy
		for (var i = 0u; i < buffer->count(); i++)
			mono_array_set(array, uint16_t, i, buffer->at(i));

		return array;
	}
}

void KeyboardBuffer::initRuntime()
{
	API_FILE("Common/KeyboardBuffer.Gen.cs");
	{
		API_COMMENT("KeyboardBuffer class.");
		API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "KeyboardBuffer");
		{
			API_COMMENT("Reads the keyboard buffer.");
			API_METHOD(PUBLIC, STATIC, "Read", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.KeyboardBuffer::Read", &Internal::read);
				
				API_RETURN("char[]");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}
