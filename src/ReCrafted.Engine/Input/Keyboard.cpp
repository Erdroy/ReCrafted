// ReCrafted (c) 2016-2018 Always Too Late

#include "Keyboard.h"

Keyboard::InputKeyState& Keyboard::GetKeyState(Key key)
{
    return m_keyStates[static_cast<int>(key)];
}

void Keyboard::UpdateKeyState(const Key key, const KeyState state)
{
    GetKeyState(key).state = state == KeyState::Down ? 1 : 0;
    m_keyStatesDirty = true;
}

Keyboard::Keyboard()
{
    // Zero-out the initial key states
    memset(m_keyStates, 0, sizeof m_keyStates);

    // Allocate some space for input string
    m_inputString.Resize(8);
}

void Keyboard::Update()
{
    // Only process new input frame, when dirty flag is set
    if (m_keyStatesDirty)
    {
        // Update all key states
        // We need to set previous key states to make IsKey(...) function possible -
        // it needs to know if the key is down and it is still pressed, when the key is UP
        // the current state is being set to 0, so then IsKey(...) returns false.
        for (rvar keyState : m_keyStates)
            keyState.previousState = keyState.state;

        // Reset dirty state
        m_keyStatesDirty = false;
    }
}

void Keyboard::LateUpdate()
{
    // Clear input string
    m_inputString.Clear();
}

void Keyboard::EmitInput(const Key key, const KeyState keyState)
{
    // Inline
    UpdateKeyState(key, keyState);
}

void Keyboard::EmitCharacter(const Char character)
{
    // Add character to the input string
    m_inputString.Add(character);
}

bool Keyboard::IsKey(const Key key)
{
    rvar state = GetKeyState(key);
    return state.state && state.previousState;
}

bool Keyboard::IsKeyDown(const Key key)
{
    rvar state = GetKeyState(key);
    return state.state && !state.previousState;
}

bool Keyboard::IsKeyUp(const Key key)
{
    rvar state = GetKeyState(key);
    return !state.state && state.previousState;
}

Char* Keyboard::GetInputString()
{
    return m_inputString.Data();
}
