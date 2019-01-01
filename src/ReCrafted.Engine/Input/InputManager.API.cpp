// ReCrafted (c) 2016-2019 Always Too Late

#include "InputManager.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    void SetCursorPosition(Vector2* position)
    {
        ASSERT(position);
        InputManager::SetCursorPosition(*position);
    }

    void GetCursorPosition(Vector2* position)
    {
        ASSERT(position);
        *position = InputManager::GetCursorPosition();
    }

    void GetCursorDelta(Vector2* delta)
    {
        ASSERT(delta);
        *delta = InputManager::GetCursorDelta();
    }

    MonoObject* GetDevice(const int deviceId)
    {
        return InputManager::GetDevice(deviceId).GetManagedPtr();
    }

    MonoObject* GetDeviceByType(const DeviceType deviceType)
    {
        return InputManager::GetDevice(deviceType).GetManagedPtr();
    }
}

void InputManager::InitRuntime()
{
    API_FILE("Input/InputManager.Gen.cs");
    {
        API_USING("ReCrafted.API.Core");
        API_USING("ReCrafted.API.Mathematics");

        API_COMMENT("InputManager class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Input", "InputManager", PARTIAL);
        {
            API_COMMENT("Returns true when specified key is being held for at least one frame.");
            API_METHOD(PUBLIC, STATIC, "IsKey", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::IsKey", &InputManager::IsKey);
                API_PARAM("Key", "key");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified key is has been pressed this frame.");
            API_METHOD(PUBLIC, STATIC, "IsKeyDown", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::IsKeyDown", &InputManager::IsKeyDown);
                API_PARAM("Key", "key");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified key is has been released this frame.");
            API_METHOD(PUBLIC, STATIC, "IsKeyUp", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::IsKeyUp", &InputManager::IsKeyUp);
                API_PARAM("Key", "key");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified button is being held for at least one frame.");
            API_METHOD(PUBLIC, STATIC, "IsButton", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::IsButton", &InputManager::IsButton);
                API_PARAM("Button", "button");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified button is has been pressed this frame.");
            API_METHOD(PUBLIC, STATIC, "IsButtonDown", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::IsButtonDown", &InputManager::IsButtonDown);
                API_PARAM("Button", "button");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Returns true when specified button is has been released this frame.");
            API_METHOD(PUBLIC, STATIC, "IsButtonUp", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::IsButtonUp", &InputManager::IsButtonUp);
                API_PARAM("Button", "button");
                API_RETURN("bool");
            }
            API_METHOD_END();

            API_COMMENT("Gets or sets the cursor position.");
            API_PROPERTY(PUBLIC, STATIC, "Vector2", "CursorPosition", GETSET, BY_REF);
            {
                API_BIND("ReCrafted.API.Input.InputManager::Internal_CursorPosition_Get", &Internal::GetCursorPosition);
                API_BIND("ReCrafted.API.Input.InputManager::Internal_CursorPosition_Set", &Internal::SetCursorPosition);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets cursor delta.");
            API_PROPERTY(PUBLIC, STATIC, "Vector2", "CursorDelta", GET, BY_REF);
            {
                API_BIND("ReCrafted.API.Input.InputManager::Internal_CursorDelta_Get", &Internal::GetCursorDelta);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets scroll delta.");
            API_PROPERTY(PUBLIC, STATIC, "float", "ScrollDelta", GET);
            {
                API_BIND("ReCrafted.API.Input.InputManager::Internal_ScrollDelta_Get", &InputManager::GetScrollDelta);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets the cursor show state.");
            API_PROPERTY(PUBLIC, STATIC, "bool", "ShowCursor", GETSET);
            {
                API_BIND("ReCrafted.API.Input.InputManager::Internal_ShowCursor_Get", &InputManager::GetShowCursor);
                API_BIND("ReCrafted.API.Input.InputManager::Internal_ShowCursor_Set", &InputManager::SetShowCursor);
            }
            API_PROPERTY_END();

            API_COMMENT("Gets or sets the cursor lock state.");
            API_PROPERTY(PUBLIC, STATIC, "bool", "LockCursor", GETSET);
            {
                API_BIND("ReCrafted.API.Input.InputManager::Internal_LockCursor_Get", &InputManager::GetLockCursor);
                API_BIND("ReCrafted.API.Input.InputManager::Internal_LockCursor_Set", &InputManager::SetLockCursor);
            }
            API_PROPERTY_END();


            API_COMMENT("Gets input device count.");
            API_METHOD(PUBLIC, STATIC, "GetDeviceCount", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::GetDeviceCount", &InputManager::GetDeviceCount);
                API_RETURN("int");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, STATIC, "GetDevice", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::GetDevice", &Internal::GetDevice);

                API_PARAM("int", "deviceId");
                API_RETURN("InputDevice");
            }
            API_METHOD_END();

            API_METHOD(INTERNAL, STATIC, "GetDevice", EXTERN);
            {
                API_BIND("ReCrafted.API.Input.InputManager::GetDevice", &Internal::GetDeviceByType);

                API_PARAM("DeviceType", "deviceType");
                API_RETURN("InputDevice");
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
