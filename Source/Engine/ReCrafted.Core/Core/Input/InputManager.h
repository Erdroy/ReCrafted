// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#pragma once

#include <ReCrafted.h>

#include "Core/SubSystems/SubSystem.h"
#include "Core/Input/ActionMap.h"
#include "Core/Input/InputDevice.h"
#include "Core/Input/NullDevice.h"
#include "Core/Input/Keyboard.h"
#include "Core/Input/Mouse.h"
#include "Core/Input/KeyboardKeys.h"
#include "Core/Input/MouseButtons.h"

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

class InputManager final : public SubSystem<InputManager>
{
    friend class Application;

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
    /**
     * \brief Returns true when specified button is being held for at least one frame.
     * \param button The button.
     */
    static bool IsButton(Button button);

    /**
     * \brief Returns true when specified button is has been pressed this frame.
     * \param button The button.
     */
    static bool IsButtonDown(Button button);

    /**
     * \brief Returns true when specified button is has been released this frame.
     * \param button The button.
     */
    static bool IsButtonUp(Button button);

    /**
     * \brief Sets mouse position relative to the current game window.
     * \param position The new position.
     */
    static void SetCursorPosition(const Vector2& position);

    /**
     * \brief Gets mouse position relative to the current game window.
     */
    static Vector2 GetCursorPosition();

    /**
     * \brief Gets mouse position delta from last frame.
     */
    static Vector2 GetCursorDelta();

    /**
     * \brief Gets mouse scroll delta from last frame.
     */
    static float GetScrollDelta();

    /**
     * \brief Sets cursor show state.
     */
    static void SetShowCursor(bool show);

    /**
     * \brief Gets cursor show state.
     */
    static bool GetShowCursor();

    /**
     * \brief Sets cursor lock state.
     */
    static void SetLockCursor(bool lock);

    /**
     * \brief Gets cursor lock state.
     */
    static bool GetLockCursor();

    /**
     * \brief Returns true when specified key is being held for at least one frame.
     * \param key The key.
     */
    static bool IsKey(Key key);

    /**
     * \brief Returns true when specified key is has been pressed this frame.
     * \param key The key.
     */
    static bool IsKeyDown(Key key);

    /**
     * \brief Returns true when specified key is has been released this frame.
     * \param key The key.
     */
    static bool IsKeyUp(Key key);

public: /* -- Devices -- */
    /**
     * \brief Finds device of given id.
     * \param deviceId The unique device identificator.
     * \return The device reference.
     * 
     * \note When there is no device with specified id, this function throws an std::exception.
     */
    static InputDevice& GetDevice(int deviceId);

    /**
     * \brief Finds device of given id and casts it into TDevice type.
     * \tparam TDevice The target device type. Eg.: Keyboard, Gamepad etc.
     * \param deviceId The unique device identificator.
     * \return The device reference.
     *
     * \note When there is no device with specified id, this function throws an std::exception.
     */
    template<typename TDevice>
    static TDevice& GetDevice(int deviceId);

    /**
     * \brief Finds default device of given device type. 
     * \param deviceType The device type.
     * \return The default device reference.
     * 
     * \note This function guarantees that it will always return proper device for
     * DeviceType::Keyboard and DeviceType::Mouse.
     * 
     * \note When there is no device with specified type (other than Mouse and Keyboard) 
     * this function throws an std::exception.
     */
    static InputDevice& GetDevice(DeviceType deviceType);

    /**
     * \brief Finds device of given type and casts it into TDevice type.
     * \tparam TDevice The target device type. Eg.: Keyboard, Gamepad etc.
     * \param deviceType The device type.
     * \return The device reference.
     * 
     * \note This function guarantees that it will always return proper device for
     * DeviceType::Keyboard and DeviceType::Mouse.
     *
     * \note When there is no device with specified type (other than Mouse and Keyboard) 
     * this function throws an std::exception.
     */
    template<typename TDevice>
    static TDevice& GetDevice(DeviceType deviceType);

    /**
     * \brief Gets device count.
     * \return The current device count.
     */
    static int GetDeviceCount();
};

template <typename TDevice>
TDevice& InputManager::GetDevice(const int deviceId)
{
    return static_cast<TDevice&>(GetDevice(deviceId));
}

template <typename TDevice>
TDevice& InputManager::GetDevice(const DeviceType deviceType)
{
    return static_cast<TDevice&>(GetDevice(deviceType));
}
