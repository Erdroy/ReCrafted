// ReCrafted (c) 2016-2018 Always Too Late

#include "Keyboard.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    bool KeyboardIsKey(Keyboard* keyboard, const Key key)
    {
        ASSERT(keyboard);
        return keyboard->IsKey(key);
    }

    bool KeyboardIsKeyDown(Keyboard* keyboard, const Key key)
    {
        ASSERT(keyboard);
        return keyboard->IsKeyDown(key);
    }

    bool KeyboardIsKeyUp(Keyboard* keyboard, const Key key)
    {
        ASSERT(keyboard);
        return keyboard->IsKeyUp(key);
    }

    MonoString* KeyboardInputString(Keyboard* keyboard)
    {
        ASSERT(keyboard);

        // Get input string
        uint32_t length;
        cvar str = keyboard->GetInputString(&length);

        // Create and return managed string
        return mono_string_new_utf16(Domain::Root->GetMono(), static_cast<mono_unichar2*>(str), static_cast<int32_t>(length));
    }
}

void Keyboard::InitRuntime()
{
    API_FILE("Input/Keyboard.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Keyboard input device");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Input", "Keyboard", "InputDevice", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("Returns true when specified key is being held for at least one frame.");
            API_METHOD(PUBLIC, REGULAR, "IsKey", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.Keyboard::Internal_IsKey", &Internal::KeyboardIsKey);
                API_PARAM("Key", "key");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified key is has been pressed this frame.");
            API_METHOD(PUBLIC, REGULAR, "IsKeyDown", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.Keyboard::Internal_IsKeyDown", &Internal::KeyboardIsKeyDown);
                API_PARAM("Key", "key");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified key is has been released this frame.");
            API_METHOD(PUBLIC, REGULAR, "IsKeyUp", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.Keyboard::Internal_IsKeyUp", &Internal::KeyboardIsKeyUp);
                API_PARAM("Key", "key");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Contains character string that has been entered during current frame.");
            API_PROPERTY(PUBLIC, REGULAR, "string", "InputString", GET);
            {
                API_BIND("ReCrafted.API.Input.Keyboard::Internal_InputString_Get", &Internal::KeyboardInputString);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
