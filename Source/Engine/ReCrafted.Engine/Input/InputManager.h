// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/SubSystems/SubSystem.h"
#include "Input/ActionMap.h"
#include "Input/InputDevice.h"
#include "Input/NullDevice.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"
#include "Input/KeyboardKeys.h"
#include "Input/MouseButtons.h"

#include <sparsepp/spp.h>

enum class InputType
{
    Unknown,

    /**
     * \brief InputData gets filled key and keyState fields.
     */
    Key,

    /**
     * \brief InputData gets filled button and buttonState fields.
     */
    Button,

    /**
     * \brief InputData gets filled axis1D/2D/3D.
     */
    Axis,

    /**
     * \brief InputData gets filled character field.
     */
    Character,

    /**
     * \brief InputData gets filled axis2D (cursorPos) and axis3D (delta -> xy).
     */
    Cursor,

    /**
     * \brief InputData gets filled axis1D.
     */
    Scroll,

    Count
};

struct InputData
{
    Vector3 axis3D;
    Vector2 axis2D;
    float axis1D;

    Char character;

    Key key;
    KeyState keyState;

    Button button;
    ButtonState buttonState;

    /**
     * \brief Used only for non-default input devices like game pads.
     */
    uint8_t deviceId = 0u;
};

API_USING("ReCrafted.API.Mathematics");

/// <summary>
///     InputManager class. Provides user input functionality.
/// </summary>
API_CLASS(public, static, partial, noinherit)
class InputManager final : public SubSystem<InputManager>
{
    friend class Application;
    API_CLASS_BODY()

    using FrameInputItem = std::pair<InputType, InputData>;

private:
    Lock m_actionMapsLock;
    spp::sparse_hash_map<std::string, ActionMap*> m_actionMaps;
    int m_actionMapCount = 0;

    Lock m_deviceMapLock;
    spp::sparse_hash_map<int, InputDevice*> m_deviceMap;
    int m_deviceCount = 0;

    Lock m_frameInputLock;
    std::vector<FrameInputItem> m_frameInput;
    std::atomic<bool> m_isEmitting;

private:
    NullDevice* m_nullDevice = nullptr;
    Keyboard* m_keyboard = nullptr;
    Mouse* m_mouse = nullptr;

protected:
    void Initialize() override;
    void Shutdown() override;

    void OnLateUpdate() override;

    void UpdateInput();
    void DispatchInput();

public:
    void BeginEmitInput();
    void EndEmitInput();
    void EmitInput(InputType type, InputData data);
    const std::vector<FrameInputItem>& GetFrameInput() const;

public: /* -- Action Maps -- */
    static ActionMap& CreateActionMap(const char* name);
    static ActionMap* GetActionMap(const char* name);
    static void DestroyActionMap(ActionMap*& actionMap);

public: /* -- Basic Input -- */

    /// <summary>
    ///     Returns true when specified button is being held for at least one frame.
    /// </summary>
    /// <param name="button">The button.</param>
    API_FUNCTION()
    static bool IsButton(Button button);

    /// <summary>
    ///    Returns true when specified button is has been pressed this frame.
    /// </summary>
    /// <param name="button">The button.</param>
    API_FUNCTION()
    static bool IsButtonDown(Button button);

    /// <summary>
    ///    Returns true when specified button is has been released this frame.
    /// </summary>
    /// <param name="button">The button.</param>
    API_FUNCTION()
    static bool IsButtonUp(Button button);

    /// <summary>
    ///     Sets mouse position relative to the current game window.
    /// </summary>
    /// <param name="position">The new position.</param>
    API_FUNCTION()
    static void SetCursorPosition(const Vector2& position);

    /// <summary>
    ///     Returns true when specified key is being held for at least one frame.
    /// </summary>
    /// <param name="key">The key.</param>
    API_FUNCTION()
    static bool IsKey(Key key);

     /// <summary>
     ///     Returns true when specified key is has been pressed this frame.
     /// </summary>
     /// <param name="key">The key.</param>
    API_FUNCTION()
    static bool IsKeyDown(Key key);

    /// <summary>
    ///     Returns true when specified key is has been released this frame.
    /// </summary>
    /// <param name="key">The key.</param>
    API_FUNCTION()
    static bool IsKeyUp(Key key);

public:
    /// <summary>
    ///     Gets mouse position relative to the current game window.
    /// </summary>
    API_PROPERTY(noprefix)
    static Vector2 GetCursorPosition();

    /// <summary>
    ///     Gets mouse position delta from last frame.
    /// </summary>
    API_PROPERTY(noprefix)
    static Vector2 GetCursorDelta();

    /// <summary>
    ///     Gets mouse scroll delta from last frame.
    /// </summary>
    API_PROPERTY(noprefix)
    static float GetScrollDelta();

    /// <summary>
    ///     Sets cursor show state.
    /// </summary>
    /// <param name="showCursor">The show state.</param>
    API_PROPERTY(noprefix)
    static void SetShowCursor(bool showCursor);

    /// <summary>
    ///     Gets cursor show state.
    /// </summary>
    API_PROPERTY(noprefix)
    static bool GetShowCursor();

    /// <summary>
    ///     Sets cursor lock state.
    /// </summary>
    /// <param name="lockCursor">The lock state.</param>
    API_PROPERTY(noprefix)
    static void SetLockCursor(bool lockCursor);

    /// <summary>
    ///     Gets cursor lock state.
    /// </summary>
    API_PROPERTY(noprefix)
    static bool GetLockCursor();

public: /* -- Devices -- */
    /// <summary>
    ///     Finds device of given id.
    /// </summary>
    /// <param name="deviceId">The unique device identificator.</param>
    /// <returns>When there is no device with specified id, this function throws an exception.</returns>
    API_FUNCTION()
    static InputDevice* GetDevice(int deviceId);

    /// <summary>
    ///     Finds device of given id and casts it into TDevice type.
    /// </summary>
    /// <typeparam name="TDevice">The target device type. Eg.: Keyboard, Gamepad etc.</typeparam>
    /// <param name="deviceId">The unique device identificator.</param>
    /// <returns>When there is no device with specified id, this function throws an exception.</returns>
    template<typename TDevice>
    static TDevice* GetDevice(int deviceId);

    /// <summary>
    ///     Finds default device of given device type.
    /// </summary>
    /// <param name="deviceType">The device type.</param>
    /// <returns>The default device.</returns>
    /// <remarks>This function guarantees that it will always return proper device for DeviceType::Keyboard and DeviceType::Mouse.</remarks>
    /// <remarks>When there is no device with specified type (other than Mouse and Keyboard) this function throws an std::exception.</remarks>
    API_FUNCTION()
    static InputDevice* GetDevice(DeviceType deviceType);

    /// <summary>
    ///     Finds default device of given device type.
    /// </summary>
    /// <typeparam name="TDevice">The target device type. Eg.: Keyboard, Gamepad etc.</typeparam>
    /// <param name="deviceType">The device type.</param>
    /// <returns>The default device.</returns>
    /// <remarks>This function guarantees that it will always return proper device for DeviceType::Keyboard and DeviceType::Mouse.</remarks>
    /// <remarks>When there is no device with specified type (other than Mouse and Keyboard) this function throws an std::exception.</remarks>
    template<typename TDevice>
    static TDevice* GetDevice(DeviceType deviceType);

    /// <summary>
    ///     Gets device count.
    /// </summary>
    API_PROPERTY(noprefix)
    static int GetDeviceCount();
};

template <typename TDevice>
TDevice* InputManager::GetDevice(const int deviceId)
{
    return static_cast<TDevice&>(GetDevice(deviceId));
}

template <typename TDevice>
TDevice* InputManager::GetDevice(const DeviceType deviceType)
{
    return static_cast<TDevice&>(GetDevice(deviceType));
}
