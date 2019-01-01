// ReCrafted (c) 2016-2019 Always Too Late

#include "Mouse.h"
#include "Scripting/ScriptingAPI.h"
#include "InputManager.h"

namespace Internal
{
    bool MouseIsButton(Mouse* mouse, const Button key)
    {
        ASSERT(mouse);
        return mouse->IsButton(key);
    }

    bool MouseIsButtonDown(Mouse* mouse, const Button key)
    {
        ASSERT(mouse);
        return mouse->IsButtonDown(key);
    }

    bool MouseIsButtonUp(Mouse* mouse, const Button key)
    {
        ASSERT(mouse);
        return mouse->IsButtonUp(key);
    }

    void MouseSetCursorPosition(Mouse* mouse, Vector2* position)
    {
        ASSERT(mouse);
        ASSERT(position);
        mouse->SetCursorPosition(*position);
    }

    void MouseGetCursorPosition(Mouse* mouse, Vector2* position)
    {
        ASSERT(mouse);
        ASSERT(position);
        *position = mouse->GetCursorPosition();
    }

    void MouseGetCursorDelta(Mouse* mouse, Vector2* delta)
    {
        ASSERT(mouse);
        ASSERT(delta);
        *delta = mouse->GetCursorDelta();
    }

    float MouseGetScrollDelta(Mouse* mouse)
    {
        ASSERT(mouse);
        return mouse->GetScrollDelta();
    }

    bool MouseGetShowCursor(Mouse* mouse)
    {
        ASSERT(mouse);
        return mouse->GetShowCursor();
    }

    void MouseSetShowCursor(Mouse* mouse, const bool show)
    {
        ASSERT(mouse);
        mouse->SetShowCursor(show);
    }

    bool MouseGetLockCursor(Mouse* mouse)
    {
        ASSERT(mouse);
        return mouse->GetLockCursor();
    }

    void MouseSetLockCursor(Mouse* mouse, const bool lock)
    {
        ASSERT(mouse);
        mouse->SetLockCursor(lock);
    }
}

void Mouse::InitRuntime()
{
    API_FILE("Input/Mouse.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("Mouse input device");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Input", "Mouse", "InputDevice", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("Returns true when specified button is being held for at least one frame.");
            API_METHOD(PUBLIC, REGULAR, "IsButton", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.Mouse::Internal_IsButton", &Internal::MouseIsButton);
                API_PARAM("Button", "button");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified button is has been pressed this frame.");
            API_METHOD(PUBLIC, REGULAR, "IsButtonDown", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.Mouse::Internal_IsButtonDown", &Internal::MouseIsButtonDown);
                API_PARAM("Button", "button");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified button is has been released this frame.");
            API_METHOD(PUBLIC, REGULAR, "IsButtonUp", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.Mouse::Internal_IsButtonUp", &Internal::MouseIsButtonUp);
                API_PARAM("Button", "button");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Gets or sets the cursor position.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector2", "CursorPosition", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Input.Mouse::Internal_CursorPosition_Get", &Internal::MouseGetCursorPosition);
                API_BIND("ReCrafted.API.Input.Mouse::Internal_CursorPosition_Set", &Internal::MouseSetCursorPosition);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets cursor delta.");
            API_PROPERTY(PUBLIC, REGULAR, "Vector2", "CursorDelta", GET, BY_REF);
            {
                API_BIND("ReCrafted.API.Input.Mouse::Internal_CursorDelta_Get", &Internal::MouseGetCursorDelta);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets scroll delta.");
            API_PROPERTY(PUBLIC, REGULAR, "float", "ScrollDelta", GET);
            {
                API_BIND("ReCrafted.API.Input.Mouse::Internal_ScrollDelta_Get", &Internal::MouseGetScrollDelta);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets the cursor show state.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "ShowCursor", GETSET);
            {
                API_BIND("ReCrafted.API.Input.Mouse::Internal_ShowCursor_Get", &Internal::MouseGetShowCursor);
                API_BIND("ReCrafted.API.Input.Mouse::Internal_ShowCursor_Set", &Internal::MouseSetShowCursor);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets the cursor lock state.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "LockCursor", GETSET);
            {
                API_BIND("ReCrafted.API.Input.Mouse::Internal_LockCursor_Get", &Internal::MouseGetLockCursor);
                API_BIND("ReCrafted.API.Input.Mouse::Internal_LockCursor_Set", &Internal::MouseSetLockCursor);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
