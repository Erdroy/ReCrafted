// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Mouse.h"
#include "Core/Display.h"

Mouse::InputButtonState& Mouse::GetButtonState(Button button)
{
    ASSERT(button != Button::Unknown);
    return m_buttonStates[static_cast<int>(button)];
}

void Mouse::UpdateButtonState(const Button button, const ButtonState state)
{
    ASSERT(button != Button::Unknown);

    GetButtonState(button).state = state == ButtonState::Down ? 1 : 0;
    m_buttonStatesDirty = true;
}

void Mouse::Update()
{
    // Only process new input frame, when dirty flag is set
    if (m_buttonStatesDirty)
    {
        // Update all button states
        // We need to set previous button states to make IsButton(...) function possible -
        // it needs to know if the button is down and it is still pressed, when the button is UP
        // the current state is being set to 0, so then IsButton(...) returns false.
        for (auto&& buttonState : m_buttonStates)
            buttonState.previousState = buttonState.state;

        // Reset dirty state
        m_buttonStatesDirty = false;
    }

    if (m_lockCursor)
    {
        // Lock cursor when needed
        SetCursorPosition(Vector2(round(Display::GetWidth() * 0.5f), round(Display::GetHeight() * 0.5f)));
    }
}

void Mouse::LateUpdate()
{
    // Cleanup cursor and scroll delta
    m_cursorDelta = Vector2::Zero;
    m_scrollDelta = 0.0f;
}

void Mouse::EmitInput(const Button button, const ButtonState buttonState)
{
    // Inline
    UpdateButtonState(button, buttonState);
}

void Mouse::EmitCursor(const Vector2 position, const Vector2 delta)
{
    m_cursorPosition = position;

    // Cursor delta must be accumulated, as there may be multiple 
    // input emits before new frame starts.
    m_cursorDelta += delta;
}

void Mouse::EmitScroll(const float delta)
{
    // Scroll delta must be accumulated, as there may be multiple 
    // input emits before new frame starts.
    m_scrollDelta += delta;
}

bool Mouse::IsButton(const Button button)
{
    auto& state = GetButtonState(button);
    return state.state && state.previousState;
}

bool Mouse::IsButtonDown(const Button button)
{
    auto& state = GetButtonState(button);
    return state.state && !state.previousState;
}

bool Mouse::IsButtonUp(const Button button)
{
    auto& state = GetButtonState(button);
    return !state.state && state.previousState;
}

void Mouse::SetCursorPosition(const Vector2& position) const
{
    Platform::SetCursorPosition(int(position.x), int(position.y));
}

Vector2 Mouse::GetCursorPosition() const
{
    return m_cursorPosition;
}

Vector2 Mouse::GetCursorDelta() const
{
    return m_cursorDelta;
}

float Mouse::GetScrollDelta() const
{
    return m_scrollDelta;
}

void Mouse::SetShowCursor(const bool cursorShow)
{
    m_showCursor = cursorShow;
    Platform::SetCursorVisibility(m_showCursor);
}

bool Mouse::GetShowCursor() const
{
    return m_showCursor;
}

void Mouse::SetLockCursor(const bool cursorLock)
{
    m_lockCursor = cursorLock;
}

bool Mouse::GetLockCursor() const
{
    return m_lockCursor;
}
