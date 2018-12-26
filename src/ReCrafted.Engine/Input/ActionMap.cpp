// ReCrafted (c) 2016-2018 Always Too Late

#include "ActionMap.h"
#include "InputManager.h"
#include "Common/Profiler/Profiler.h"

ActionMap::ActionItem* ActionMap::GetActionItem(const char* name)
{
    ScopeLock(m_actionItemsLock);

    crvar it = m_actionItemMap.find(std::string(name));
    if (it == m_actionItemMap.end())
        return nullptr;
    return &it->second;
}

void ActionMap::CallStateListeners(ActionItem* action, const bool down, const bool up, const bool held)
{
    InputState inputState{};
    inputState.pressed = down;
    inputState.released = up;
    //inputState.held = held;

    for (rvar listener : action->stateListeners)
    {
        listener.Invoke(inputState);
    }
}

void ActionMap::DoKeyActions(Key key, KeyState state)
{
    cvar keyCode = static_cast<int>(key);
    if (!m_keyBinds[keyCode].empty())
    {
        for (rvar action : m_keyBinds[keyCode])
        {
            switch (action->type)
            {
            case ActionType::Event:
            {
                if((static_cast<int>(action->eventType) & static_cast<int>(state)) != 0)
                {
                    for(rvar listener : action->eventListeners)
                    {
                        listener.Invoke();
                    }
                }
                return;
            }
            case ActionType::State:
            {
                CallStateListeners(action, state == KeyState::Down, state == KeyState::Up, InputManager::IsKey(key));
                return;
            }
            default: return;
            }
        }
    }
}

void ActionMap::DoButtonActions(Button button, ButtonState state)
{
    cvar buttonCode = static_cast<int>(button);
    if (!m_buttonBinds[buttonCode].empty())
    {
        for (rvar action : m_buttonBinds[buttonCode])
        {
            switch (action->type)
            {
            case ActionType::Event:
            {
                if ((static_cast<int>(action->eventType) & static_cast<int>(state)) != 0)
                {
                    for (rvar listener : action->eventListeners)
                    {
                        listener.Invoke();
                    }
                }
                return;
            }
            case ActionType::State:
            {
                CallStateListeners(action, state == ButtonState::Down, state == ButtonState::Up, InputManager::IsButton(button));
                return;
            }
            default: return;
            }
        }
    }
}

void ActionMap::Update()
{
    CPU_PROFILE_SCOPE(0, __FUNCTION__);

    if (!m_active)
        return;

    rvar inputList = InputManager::GetInstance()->GetFrameInput();
    // TODO: Add second frame input buffer to store previous key state etc. (same thing is in Mouse/Keyboard devices)
    for(rvar input : inputList)
    {
        switch(input.first)
        {
        case InputType::Key:
        {
            DoKeyActions(input.second.key, input.second.keyState);
            break;
        }
        case InputType::Button:
        {
            DoButtonActions(input.second.button, input.second.buttonState);
            break;
        }
        case InputType::Axis:
        case InputType::Character:
        case InputType::Cursor:
        case InputType::Scroll:
        case InputType::Unknown:
        case InputType::Count:
        default: break;
        }
    }
}

void ActionMap::SetActive(const bool active)
{
    m_active = active;
}

bool ActionMap::IsActive() const
{
    return m_active;
}

void ActionMap::AddAction(const char* name, const ActionType actionType, const ActionEventType eventType)
{
    ScopeLock(m_actionItemsLock);

    ActionItem item;
    item.type = actionType;
    item.eventType = eventType;
    item.name = name;

    // Insert new action item
    m_actionItemMap.insert(std::make_pair(std::string(name), item));
}

void ActionMap::RemoveAction(const char* name)
{
    ScopeLock(m_actionItemsLock);

    // Find and remove action item if found
    cvar it = m_actionItemMap.find(std::string(name));
    if (it != m_actionItemMap.end())
        m_actionItemMap.erase(it);
}

void ActionMap::AddBind(const char* name, const Key key)
{
    cvar action = GetActionItem(name);
    if (!action)
        return;

    // Add key bind
    m_keyBinds[static_cast<int>(key)].push_back(action);
}

void ActionMap::AddBind(const char* name, const Button button)
{
    cvar action = GetActionItem(name);
    if (!action)
        return;

    // Add button bind
    m_buttonBinds[static_cast<int>(button)].push_back(action);
}

void ActionMap::RemoveBinds(const char* name)
{
    cvar action = GetActionItem(name);
    if (!action)
        return;

    // Clear key and button binds for selected action
    for(rvar bind : m_keyBinds)
    {
        cvar it = std::find(bind.begin(), bind.end(), action);
        if(it != bind.end())
            bind.erase(it);
    }
    
    for (rvar bind : m_buttonBinds)
    {
        cvar it = std::find(bind.begin(), bind.end(), action);
        if (it != bind.end())
            bind.erase(it);
    }
}

void ActionMap::AddListener(const char* name, const Action<void>& action)
{
    cvar actionItem = GetActionItem(name);
    if (!actionItem)
        return;

    // Add listener
    actionItem->eventListeners.push_back(action);
}

void ActionMap::AddListener(const char* name, const Action<void, InputState>& action)
{
    cvar actionItem = GetActionItem(name);
    if (!actionItem)
        return;

    // Add listener
    actionItem->stateListeners.push_back(action);
}

void ActionMap::AddListener(const char* name, const Action<void, float>& action)
{
    cvar actionItem = GetActionItem(name);
    if (!actionItem)
        return;

    // Add listener
    actionItem->axis1DListeners.push_back(action);
}

void ActionMap::AddListener(const char* name, const Action<void, Vector2>& action)
{
    cvar actionItem = GetActionItem(name);
    if (!actionItem)
        return;

    // Add listener
    actionItem->axis2DListeners.push_back(action);
}

void ActionMap::AddListener(const char* name, const Action<void, Vector3>& action)
{
    cvar actionItem = GetActionItem(name);
    if (!actionItem)
        return;

    // Add listener
    actionItem->axis3DListeners.push_back(action);
}
