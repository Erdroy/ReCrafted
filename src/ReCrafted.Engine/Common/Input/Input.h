// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef INPUT_H
#define INPUT_H

// includes
#include "ReCrafted.h"
#include "Core/EngineComponent.h"
#include "Core/Math/math.h"
#include "Platform/Platform.h"

#define INPUT_KEYCOUNT 256
#define INPUT_TABLE_SIZE INPUT_KEYCOUNT + 3

#define INPUT_LBUTTON INPUT_KEYCOUNT + 0
#define INPUT_MBUTTON INPUT_KEYCOUNT + 1
#define INPUT_RBUTTON INPUT_KEYCOUNT + 2

#define INPUT_KEY_TO_KEYID(key) key < 0x4 ? uint(INPUT_KEYCOUNT + key) : uint(key);

enum Keys;

/**
 * \brief Input class.
 */
class Input : public EngineComponent<Input>
{
    friend class EngineMain;

private:
SCRIPTING_API_IMPL()

private:
    byte m_lastkeys[INPUT_TABLE_SIZE] = {};
    byte m_keys[INPUT_TABLE_SIZE] = {};

    Vector2 m_cursorPos = {};
    Vector2 m_cursorDelta = {};
    float m_scrollDelta_u = 0.0f;
    float m_scrollDelta = 0.0f;

    bool m_lockCursor = false;
    bool m_showCursor = true;

private:
    void onInit() override;
    void onDispose() override;

    void updateInput();

public:
    virtual ~Input() = default;

public:
    // INTERNAL
    void emit(bool up, uint key);

    // INTERNAL
    void emitScroll(float delta);

    // INTERNAL
    void emitMouseMove(int deltaX, int deltaY, int cursorX, int cursorY);

    // INTERNAL
    // releases all keys
    void releaseAll();

public:
    /**
	 * \brief Checks if key is down.
	 * \param key The key
	 * \return Returns true when key is down.
	 */
    FORCEINLINE static bool isKeyDown(Keys key)
    {
        auto keyId = INPUT_KEY_TO_KEYID(key);
        return m_instance->m_keys[keyId] == 1 && m_instance->m_lastkeys[keyId] == 0;
    }

    /**
    * \brief Checks if key is up.
    * \param key The key
    * \return Returns true when key is up.
    */
    FORCEINLINE static bool isKeyUp(Keys key)
    {
        auto keyId = INPUT_KEY_TO_KEYID(key);
        return m_instance->m_keys[keyId] == 0 && m_instance->m_lastkeys[keyId] == 1;
    }

    /**
    * \brief Checks if key is held.
    * \param key The key
    * \return Returns true when key is held.
    */
    FORCEINLINE static bool isKey(Keys key)
    {
        auto keyId = INPUT_KEY_TO_KEYID(key);
        return m_instance->m_keys[keyId] == 1 && m_instance->m_lastkeys[keyId] == 1;
    }

    /**
	 * \brief Returns current cursor's position in client-space.
	 * \return The position position.
	 */
    FORCEINLINE static Vector2 getCursorPos()
    {
        return m_instance->m_cursorPos;
    }

    /**
	 * \brief Returns current cursor's delta.
	 * \return The position.
	 */
    FORCEINLINE static Vector2 getCursorDelta()
    {
        return m_instance->m_cursorDelta;
    }

    /**
     * \brief Returns current scroll (mouse wheel) delta.
     * \return The scrolls delta.
     */
    FORCEINLINE static float getScrollDelta()
    {
        return m_instance->m_scrollDelta;
    }

    /**
	 * \brief Sets cursor position.
	 * \param x The x coord of target cursor position in pixels.
	 * \param y The y coord of target cursor position in pixels.
	 */
    FORCEINLINE static void setCursorPos(int x, int y)
    {
        Platform::setCursorPosition(uint16_t(x), uint16_t(y));
    }

    /**
     * \brief Gets cursor lock state.
     */
    FORCEINLINE static bool getLockCursor()
    {
        return m_instance->m_lockCursor;
    }

    /**
    * \brief Sets cursor lock state.
    */
    FORCEINLINE static void setLockCursor(bool lock)
    {
        m_instance->m_lockCursor = lock;
    }

    /**
    * \brief Gets cursor visibility state.
    */
    FORCEINLINE static bool getShowCursor()
    {
        return m_instance->m_showCursor;
    }

    /**
    * \brief Sets cursor visibility state.
    */
    FORCEINLINE static void setShowCursor(bool show)
    {
        m_instance->m_showCursor = show;
    }
};

enum Keys
{
    Key_Mouse0 = 0x0,
    Key_Mouse1 = 0x2,
    Key_Mouse2 = 0x1,

    Key_Backspace = 0x08,
    Key_Tab = 0x09,
    Key_Clear = 0x0C,
    Key_Enter = 0x0D,
    Key_Shift = 0x10,
    Key_Control = 0x11,
    Key_Alt = 0x12,
    Key_Pause = 0x13,
    Key_CapsLock = 0x14,
    Key_Escape = 0x1B,
    Key_Space = 0x20,
    Key_PageUp = 0x21,
    Key_PageDown = 0x22,
    Key_End = 0x23,
    Key_Home = 0x24,
    Key_Left = 0x25,
    Key_Up = 0x26,
    Key_Right = 0x27,
    Key_Down = 0x28,
    Key_Select = 0x29,
    Key_Print = 0x2A,
    Key_Execute = 0x2B,
    Key_PrintScreen = 0x2C,
    Key_Insert = 0x2D,
    Key_Delete = 0x2E,
    Key_Help = 0x2F,

    Key_Alpha0 = 0x30,
    Key_Alpha1 = 0x31,
    Key_Alpha2 = 0x32,
    Key_Alpha3 = 0x33,
    Key_Alpha4 = 0x34,
    Key_Alpha5 = 0x35,
    Key_Alpha6 = 0x36,
    Key_Alpha7 = 0x37,
    Key_Alpha8 = 0x38,
    Key_Alpha9 = 0x39,

    Key_A = 0x41,
    Key_B = 0x42,
    Key_C = 0x43,
    Key_D = 0x44,
    Key_E = 0x45,
    Key_F = 0x46,
    Key_G = 0x47,
    Key_H = 0x48,
    Key_I = 0x49,
    Key_J = 0x4A,
    Key_K = 0x4B,
    Key_L = 0x4C,
    Key_M = 0x4D,
    Key_N = 0x4E,
    Key_O = 0x4F,
    Key_P = 0x50,
    Key_Q = 0x51,
    Key_R = 0x52,
    Key_S = 0x53,
    Key_T = 0x54,
    Key_U = 0x55,
    Key_V = 0x56,
    Key_W = 0x57,
    Key_X = 0x58,
    Key_Y = 0x59,
    Key_Z = 0x5A,

    Key_LeftWindowsKey = 0x5B,
    Key_RightWindowsKey = 0x5C,
    Key_ApplicationsKey = 0x5D,
    Key_Sleep = 0x5F,
    Key_NumPad0 = 0x60,
    Key_NumPad1 = 0x61,
    Key_NumPad2 = 0x62,
    Key_NumPad3 = 0x63,
    Key_NumPad4 = 0x64,
    Key_NumPad5 = 0x65,
    Key_NumPad6 = 0x66,
    Key_NumPad7 = 0x67,
    Key_NumPad8 = 0x68,
    Key_NumPad9 = 0x69,
    Key_Multiply = 0x6A,
    Key_Add = 0x6B,
    Key_Seperator = 0x6C,
    Key_Subtract = 0x6D,
    Key_Decimal = 0x6E,
    Key_Divide = 0x6F,
    Key_F1 = 0x70,
    Key_F2 = 0x71,
    Key_F3 = 0x72,
    Key_F4 = 0x73,
    Key_F5 = 0x74,
    Key_F6 = 0x75,
    Key_F7 = 0x76,
    Key_F8 = 0x77,
    Key_F9 = 0x78,
    Key_F10 = 0x79,
    Key_F11 = 0x7A,
    Key_F12 = 0x7B,
    Key_F13 = 0x7C,
    Key_F14 = 0x7D,
    Key_F15 = 0x7E,
    Key_F16 = 0x7F,
    Key_F17 = 0x80,
    Key_F18 = 0x81,
    Key_F19 = 0x82,
    Key_F20 = 0x83,
    Key_F21 = 0x84,
    Key_F22 = 0x85,
    Key_F23 = 0x86,
    Key_F24 = 0x87,
    Key_Numlock = 0x90,
    Key_ScrollLock = 0x91,
    Key_LeftShift = 0xA0,
    Key_RightShift = 0xA1,
    Key_LeftControl = 0xA2,
    Key_RightContol = 0xA3,
    Key_LeftMenu = 0xA4,
    Key_RightMenu = 0xA5,
    Key_BrowserBack = 0xA6,
    Key_BrowserForward = 0xA7,
    Key_BrowserRefresh = 0xA8,
    Key_BrowserStop = 0xA9,
    Key_BrowserSearch = 0xAA,
    Key_BrowserFavorites = 0xAB,
    Key_BrowserHome = 0xAC,
    Key_VolumeMute = 0xAD,
    Key_VolumeDown = 0xAE,
    Key_VolumeUp = 0xAF,
    Key_NextTrack = 0xB0,
    Key_PreviousTrack = 0xB1,
    Key_StopMedia = 0xB2,
    Key_PlayPause = 0xB3,
    Key_LaunchMail = 0xB4,
    Key_SelectMedia = 0xB5,
    Key_LaunchApp1 = 0xB6,
    Key_LaunchApp2 = 0xB7,
    Key_OEM1 = 0xBA,
    Key_OEMPlus = 0xB8,
    Key_OEMComma = 0xBC,
    Key_OEMMinus = 0xBD,
    Key_OEMPeriod = 0xBE,
    Key_OEM2 = 0xBF,
    Key_OEM3 = 0xC0,
    Key_OEM4 = 0xDB,
    Key_OEM5 = 0xDC,
    Key_OEM6 = 0xDD,
    Key_OEM7 = 0xDE,
    Key_OEM8 = 0xDF,
    Key_OEM102 = 0xE2,
    Key_Process = 0xE5,
    Key_Packet = 0xE7,
    Key_Attn = 0xF6,
    Key_CrSel = 0xF7,
    Key_ExSel = 0xF8,
    Key_EraseEOF = 0xF9,
    Key_Play = 0xFA,
    Key_Zoom = 0xFB,
    Key_PA1 = 0xFD,
    Key_OEMClear = 0xFE,
};

#endif // INPUT_H
