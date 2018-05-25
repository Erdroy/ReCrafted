// ReCrafted (c) 2016-2018 Always Too Late

#include "Input.h"
#include "Scripting/Mono.h"
#include "Common/ReCraftedAPI.h"

namespace Internal
{
    bool IsKeyDown(int key)
    {
        return Input::IsKeyDown(static_cast<Keys>(key));
    }

    bool IsKeyUp(int key)
    {
        return Input::IsKeyUp(static_cast<Keys>(key));
    }

    bool IsKey(int key)
    {
        return Input::IsKey(static_cast<Keys>(key));
    }

    void GetCursorPos(Vector2* position)
    {
        *position = Input::GetCursorPos();
    }

    void SetCursorPos(Vector2* position)
    {
        Input::SetCursorPos(static_cast<int>(position->x), static_cast<int>(position->y));
    }

    void SetCursorDelta(Vector2* delta)
    {
        *delta = Input::GetCursorDelta();
    }

    void GetCursorDelta(Vector2* delta)
    {
        *delta = Input::GetCursorDelta();
    }
}

void Input::InitRuntime()
{
    API_FILE("Common/Input.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Input class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "Input");
        {
            API_COMMENT("Checks if the key is pressed down.");
            API_METHOD(PUBLIC, STATIC, "IsKeyDown", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Input::IsKeyDown", &Internal::IsKeyDown);
                API_COMMENT("The key");
                API_PARAM("Keys", "key");

                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Checks if the key is not pressed.");
            API_METHOD(PUBLIC, STATIC, "IsKeyUp", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Input::IsKeyUp", &Internal::IsKeyUp);
                API_COMMENT("The key");
                API_PARAM("Keys", "key");

                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Checks if the key held.");
            API_METHOD(PUBLIC, STATIC, "IsKey", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Input::IsKey", &Internal::IsKey);
                API_COMMENT("The key");
                API_PARAM("Keys", "key");

                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns cursor's current position.");
            API_PROPERTY(PUBLIC, STATIC, "Vector2", "CursorPosition", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Common.Input::Internal_CursorPosition_Get", &Internal::GetCursorPos);
                API_BIND("ReCrafted.API.Common.Input::Internal_CursorPosition_Set", &Internal::SetCursorPos);
            }
            API_PROPERTY_END();

            API_COMMENT("Returns cursor's position change since last frame.");
            API_PROPERTY(PUBLIC, STATIC, "Vector2", "CursorDelta", GET, BY_REF);
            {
                API_BIND("ReCrafted.API.Common.Input::Internal_CursorDelta_Get", &Internal::GetCursorDelta);
            }
            API_PROPERTY_END();

            API_COMMENT("Returns current scroll (mouse wheel) delta.");
            API_PROPERTY(PUBLIC, STATIC, "float", "ScrollDelta", GET);
            {
                API_BIND("ReCrafted.API.Common.Input::Internal_ScrollDelta_Get", &Input::GetScrollDelta);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets the cursor lock state");
            API_PROPERTY(PUBLIC, STATIC, "bool", "LockCursor", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Input::Internal_LockCursor_Get", &Input::GetLockCursor);
                API_BIND("ReCrafted.API.Common.Input::Internal_LockCursor_Set", &Input::SetLockCursor);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets the cursor visibility state");
            API_PROPERTY(PUBLIC, STATIC, "bool", "ShowCursor", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Input::Internal_ShowCursor_Get", &Input::GetShowCursor);
                API_BIND("ReCrafted.API.Common.Input::Internal_ShowCursor_Set", &Input::SetShowCursor);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
