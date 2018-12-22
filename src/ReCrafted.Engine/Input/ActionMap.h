// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTIONMAP_H
#define ACTIONMAP_H

#include "ReCrafted.h"
#include "Core/Action.h"
#include "Core/Lock.h"
#include "Input/KeyboardKeys.h"
#include "Input/MouseButtons.h"
#include "Scripting/Object.h"

enum class AxisFiltering : byte
{
    /**
     * \brief No filtering for action.
     */
    None,

    /**
     * \brief Use a number of previous frames to filter-out the input.
     */
    OverFrames
};

enum class ActionType : byte
{
    Event,

    State,

    Axis1D,
    Axis2D,
    Axis3D
};

enum class ActionEventType : byte
{
    None,
    Pressed = 1 << 0,
    Released = 1 << 1,
    Held = 1 << 2
};

struct InputState
{
    bool pressed : 1;
    bool held : 1;
    bool released : 1;
};
static_assert(sizeof(InputState), "InputState structure size must be 1 byte long");

class ActionMap : public Object
{
    friend class InputManager;

private:
    struct ActionItem
    {
        std::string name;
        ActionType type;
        ActionEventType eventType;

        std::vector<Key> keyBinds;
        std::vector<Button> buttonBinds;

        std::vector<Action<void>> eventListeners;
        std::vector<Action<void, InputState>> stateListeners;
        std::vector<Action<void, float>> axis1DListeners;
        std::vector<Action<void, Vector2>> axis2DListeners;
        std::vector<Action<void, Vector3>> axis3DListeners;
    };

private:
    SCRIPTING_API_IMPL();

private:
    bool m_active = true;
    std::string m_name;

    spp::sparse_hash_map<std::string, ActionItem> m_actionItemMap;
    Lock m_actionItemsLock;

private:
    void SetName(const std::string& name)
    {
        m_name = name;
    }

    std::string& GetName()
    {
        return m_name;
    }

    ActionItem* GetActionItem(const char* name);

public:
    ActionMap() = default;
    ActionMap(ActionMap& other) = delete;
    ActionMap(ActionMap&& other) = delete;
    ~ActionMap() override = default;

private:
    void Update();

protected:
    void OnDestroy() override { }

    const char* GetObjectName() const override
    {
        return "ActionMap";
    }

public:
    void SetActive(bool active);
    bool IsActive() const;

public:
    void AddAction(const char* name, ActionType actionType, ActionEventType eventType = ActionEventType::None);
    void RemoveAction(const char* name);

public:
    void AddBind(const char* name, Key key);
    void AddBind(const char* name, Button button);
    void RemoveBinds(const char* name);

public:
    void AddListener(const char* name, const Action<void>& action);
    void AddListener(const char* name, const Action<void, InputState>& action);
    void AddListener(const char* name, const Action<void, float>& action);
    void AddListener(const char* name, const Action<void, Vector2>& action);
    void AddListener(const char* name, const Action<void, Vector3>& action);

public:
    bool operator[](const char* name) const {} // TODO: Manual listening implementation
    
    ActionMap& operator=(const ActionMap& other) = delete;
};

#endif // ACTIONMAP_H
