// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Common/List.h"
#include "Input/InputDevice.h"
#include "Input/KeyboardKeys.h"

/**
 * \brief Keyboard input device.
 */
class Keyboard final : public InputDevice
{
    friend class InputManager;

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
    Char* GetInputString(uint32_t* length);
};
