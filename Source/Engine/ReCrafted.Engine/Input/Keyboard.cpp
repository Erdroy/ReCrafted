// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

#include "Keyboard.h"

Keyboard::InputKeyState& Keyboard::GetKeyState(Key key)
{
    ASSERT(key != Key::Unknown);

    return m_keyStates[static_cast<int>(key)];
}

void Keyboard::UpdateKeyState(const Key key, const KeyState state)
{
    ASSERT(key != Key::Unknown);

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

Keyboard::~Keyboard()
{
    m_inputString.Release();
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
        for (auto&& keyState : m_keyStates)
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
    auto& state = GetKeyState(key);
    return state.state && state.previousState;
}

bool Keyboard::IsKeyDown(const Key key)
{
    auto& state = GetKeyState(key);
    return state.state && !state.previousState;
}

bool Keyboard::IsKeyUp(const Key key)
{
    auto& state = GetKeyState(key);
    return !state.state && state.previousState;
}

Char* Keyboard::GetInputString(uint32_t* length)
{
    *length = static_cast<uint32_t>(m_inputString.Size());
    return m_inputString.Data();
}
