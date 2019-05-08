// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Input/InputDevice.h"
#include "Input/KeyboardKeys.h"

/// <summary>
///     Keyboard input device.
/// </summary>
API_CLASS(public)
class Keyboard final : public InputDevice
{
    friend class InputManager;
    API_CLASS_BODY()

public:
    /// <summary>
    ///     The amount of (base) keys that are supported by Keyboard input device.
    /// </summary>
    /// <remarks>Does not include modified (special) keys.</remarks>
    static const int KeyCount = static_cast<int>(Key::Count);

private:
    struct InputKeyState
    {
    public:
        uint8_t previousState : 1;
        uint8_t state : 1;

        uint8_t reserved : 6;
    };
    static_assert(sizeof(InputKeyState) == 1u, "InputKeyState structure must be 1 byte long!");

private:
    InputKeyState m_keyStates[KeyCount] = {};
    bool m_keyStatesDirty = false;

    List<Char> m_inputString = {};

private:
    FORCE_INLINE InputKeyState& GetKeyState(Key key);
    FORCE_INLINE void UpdateKeyState(Key key, KeyState state);

public:
    Keyboard();
    ~Keyboard() override;

protected:
    void Update() override;
    void LateUpdate() override;

public:
    /// <summary>
    ///     Emits given key and it's state.
    /// </summary>
    /// <param name="key">The key.</param>
    /// <param name="keyState">The key state.</param>
    API_FUNCTION()
    void EmitInput(Key key, KeyState keyState);

    /// <summary>
    ///     Emits given character.
    /// </summary>
    /// <param name="character">The character.</param>
    API_FUNCTION()
    void EmitCharacter(Char character);

public:
    /// <summary>
    ///     The name of this device.
    /// </summary>
    API_FUNCTION()
    const char* DeviceName() override
    {
        return "Default Keyboard";
    }

    /// <summary>
    ///     The type of this device.
    /// </summary>
    API_FUNCTION()
    DeviceType Type() override
    {
        return DeviceType::Keyboard;
    }

public:
    /// <summary>
    ///     Returns true when specified key is being held for at least one frame.
    /// </summary>
    /// <param name="key">The key.</param>
    bool IsKey(Key key);

    /// <summary>
    ///     Returns true when specified key is has been pressed this frame.
    /// </summary>
    /// <param name="key">The key.</param>
    bool IsKeyDown(Key key);

    /// <summary>
    ///     Returns true when specified key is has been released this frame.
    /// </summary>
    /// <param name="key">The key.</param>
    bool IsKeyUp(Key key);

public:
    /**
     * \brief Contains character string that has been entered during current frame.
     */
    Char* GetInputString(uint32_t* length);
};
