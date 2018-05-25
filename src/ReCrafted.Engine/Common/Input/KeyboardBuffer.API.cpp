// ReCrafted (c) 2016-2018 Always Too Late

#include "KeyboardBuffer.h"
#include "ReCrafted.h"
#include "Common/ReCraftedAPI.h"
#include "Scripting/Mono.h"

namespace Internal
{
    MonoArray* Read()
    {
        var buffer = KeyboardBuffer::GetBuffer();

        var uintClass = mono_get_uint16_class();
        var array = mono_array_new(Domain::Root->GetMono(), uintClass, buffer->Count());

        // copy
        for (var i = 0u; i < buffer->Count(); i++)
            mono_array_set(array, uint16_t, i, buffer->At(i));

        return array;
    }
}

void KeyboardBuffer::InitRuntime()
{
    API_FILE("Common/KeyboardBuffer.Gen.cs");
    {
        API_COMMENT("KeyboardBuffer class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "KeyboardBuffer");
        {
            API_COMMENT("Reads the keyboard buffer.");
            API_METHOD(PUBLIC, STATIC, "Read", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.KeyboardBuffer::Read", &Internal::Read);

                API_RETURN("char[]");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
