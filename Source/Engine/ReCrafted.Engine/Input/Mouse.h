// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Input/InputDevice.h"
#include "Input/MouseButtons.h"

API_USING("ReCrafted.API.Mathematics")

/// <summary>
///     Mouse input device.
/// </summary>
API_CLASS(public)
class Mouse final : public InputDevice
{
    friend class InputManager;
    API_CLASS_BODY()

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
    /// <summary>
    ///     Emits given button and it's state.
    /// </summary>
    /// <param name="button">The button.</param>
    /// <param name="buttonState">The button state.</param>
    API_FUNCTION()
    void EmitInput(Button button, ButtonState buttonState);

    /// <summary>
    ///     Emits cursor move.
    /// </summary>
    /// <param name="position">The new position.</param>
    /// <param name="delta">The cursor delta.</param>
    API_FUNCTION()
    void EmitCursor(Vector2 position, Vector2 delta);

    /// <summary>
    ///     Emits scroll move.
    /// </summary>
    /// <param name="delta">The scroll delta.</param>
    API_FUNCTION()
    void EmitScroll(float delta);

public:
    /// <summary>
    ///     The name of this device.
    /// </summary>
    API_FUNCTION()
    const char* DeviceName() override
    {
        return "Default Mouse";
    }

    /// <summary>
    ///     The type of this device.
    /// </summary>
    API_FUNCTION()
    DeviceType Type() override
    {
        return DeviceType::Mouse;
    }

public:
    /// <summary>
    ///     Returns true when specified button is being held for at least one frame.
    /// </summary>
    /// <param name="button">The button.</param>
    API_FUNCTION()
    bool IsButton(Button button);

    /// <summary>
    ///     Returns true when specified button is has been pressed this frame.
    /// </summary>
    /// <param name="button">The button.</param>
    API_FUNCTION()
    bool IsButtonDown(Button button);

    /// <summary>
    ///     Returns true when specified button is has been released this frame.
    /// </summary>
    /// <param name="button">The button.</param>
    API_FUNCTION()
    bool IsButtonUp(Button button);

public:
    /// <summary>
    ///     Sets mouse position relative to the current game window.
    /// </summary>
    /// <param name="position">The new position.</param>
    API_FUNCTION()
    void SetCursorPosition(const Vector2& position) const;

    /// <summary>
    ///     Gets mouse position relative to the current game window.
    /// </summary>
    API_FUNCTION()
    Vector2 GetCursorPosition() const;

    /// <summary>
    ///     Gets mouse position delta from last frame.
    /// </summary>
    API_FUNCTION()
    Vector2 GetCursorDelta() const;

    /// <summary>
    ///     Gets mouse scroll delta from last frame.
    /// </summary>
    API_FUNCTION()
    float GetScrollDelta() const;

    /// <summary>
    ///     Sets cursor show state.
    /// </summary>
    API_FUNCTION()
    void SetShowCursor(bool cursorShow);

    /// <summary>
    ///     Gets cursor show state.
    /// </summary>
    API_FUNCTION()
    bool GetShowCursor() const;

    /// <summary>
    ///     Sets cursor lock state.
    /// </summary>
    API_FUNCTION()
    void SetLockCursor(bool cursorLock);

    /// <summary>
    ///     Gets cursor lock state.
    /// </summary>
    API_FUNCTION()
    bool GetLockCursor() const;
};
