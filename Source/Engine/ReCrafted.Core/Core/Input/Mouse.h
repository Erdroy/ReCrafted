// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/Input/InputDevice.h"
#include "Core/Input/MouseButtons.h"

/**
 * \brief Mouse input device.
 */
class Mouse final : public InputDevice
{
    friend class InputManager;

public:
    /**
     * \brief The amount of (base) buttons that are supported by Mouse input device.
     */
    static const int ButtonCount = static_cast<int>(Button::Count);

private:
    struct InputButtonState
    {
    public:
        uint8_t previousState : 1;
        uint8_t state : 1;

        uint8_t reserved : 6;
    };
    static_assert(sizeof(InputButtonState) == 1u, "InputButtonState structure must be 1 byte long!");

private:
    InputButtonState m_buttonStates[ButtonCount] = {};
    bool m_buttonStatesDirty = false;

    bool m_lockCursor = false;
    bool m_showCursor = true;

    Vector2 m_lastCursorPosition = Vector2::Zero;
    Vector2 m_cursorPosition = Vector2::Zero;
    Vector2 m_cursorDelta = Vector2::Zero;
    float m_scrollDelta = 0.0f;

private:
    FORCE_INLINE InputButtonState& GetButtonState(Button button);
    FORCE_INLINE void UpdateButtonState(Button button, ButtonState state);

public:
    Mouse() = default;
    ~Mouse() override = default;

protected:
    void Update() override;
    void LateUpdate() override;

public:
    void EmitInput(Button button, ButtonState buttonState);
    void EmitCursor(Vector2 position, Vector2 delta);
    void EmitScroll(float delta);

public:
    /**
     * \brief The name of this device.
     */
    const char* Name() override
    {
        return "Default Mouse";
    }

    /**
     * \brief The type of this device.
     */
    DeviceType Type() override
    {
        return DeviceType::Mouse;
    }

public:
    /**
     * \brief Returns true when specified button is being held for at least one frame.
     * \param button The button.
     */
    bool IsButton(Button button);

    /**
     * \brief Returns true when specified button is has been pressed this frame.
     * \param button The button.
     */
    bool IsButtonDown(Button button);

    /**
     * \brief Returns true when specified button is has been released this frame.
     * \param button The button.
     */
    bool IsButtonUp(Button button);

public:
    /**
     * \brief Sets mouse position relative to the current game window.
     * \param position The new position.
     */
    void SetCursorPosition(const Vector2& position) const;

    /**
     * \brief Gets mouse position relative to the current game window.
     */
    Vector2 GetCursorPosition() const;

    /**
     * \brief Gets mouse position delta from last frame.
     */
    Vector2 GetCursorDelta() const;

    /**
     * \brief Gets mouse scroll delta from last frame.
     */
    float GetScrollDelta() const;

    /**
     * \brief Sets cursor show state.
     */
    void SetShowCursor(bool show);

    /**
     * \brief Gets cursor show state.
     */
    bool GetShowCursor() const;

    /**
     * \brief Sets cursor lock state.
     */
    void SetLockCursor(bool lock);

    /**
     * \brief Gets cursor lock state.
     */
    bool GetLockCursor() const;
};
