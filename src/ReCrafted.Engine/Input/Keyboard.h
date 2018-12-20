// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "ReCrafted.h"
#include "Core/Containers/Array.h"
#include "Input/InputDevice.h"
#include "Input/KeyboardKeys.h"

/**
 * \brief Keyboard input device.
 */
class Keyboard : public InputDevice
{
    friend class InputManager;

private:
    SCRIPTING_API_IMPL();

public:
    /**
     * \brief The amount of (base) keys that are supported by Keyboard input device.
     * \note Does not include modified (special) keys.
     */
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

    Array<Char> m_inputString = {};

private:
    FORCEINLINE InputKeyState& GetKeyState(Key key);
    FORCEINLINE void UpdateKeyState(Key key, KeyState state);

public:
    Keyboard();
    ~Keyboard() override = default;

protected:
    void Update() override;
    void LateUpdate() override;

protected:
    void OnDestroy() override { }

    const char* GetObjectName() const override
    {
        return "Keyboard (InputDevice)";
    }

public:
    void EmitInput(Key key, KeyState keyState);
    void EmitCharacter(Char character);

public:
    /**
     * \brief The name of this device.
     */
    const char* Name() override
    {
        return "Default Keyboard";
    }

    /**
     * \brief The type of this device.
     */
    DeviceType Type() override
    {
        return DeviceType::Keyboard;
    }

public:
    /**
     * \brief Returns true when specified key is being held for at least one frame.
     * \param key The key.
     */
    bool IsKey(Key key);

    /**
     * \brief Returns true when specified key is has been pressed this frame.
     * \param key The key.
     */
    bool IsKeyDown(Key key);

    /**
     * \brief Returns true when specified key is has been released this frame.
     * \param key The key.
     */
    bool IsKeyUp(Key key);

public:
    /**
     * \brief Contains character string that has been entered during current frame.
     */
    Char* GetInputString();
};

#endif // KEYBOARD_H
