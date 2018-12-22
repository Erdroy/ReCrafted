// ReCrafted (c) 2016-2018 Always Too Late

#include "ActionMap.h"
#include "InputManager.h"

ActionMap::ActionItem* ActionMap::GetActionItem(const char* name)
{
    ScopeLock(m_actionItemsLock);

    crvar it = m_actionItemMap.find(std::string(name));
    if (it == m_actionItemMap.end())
        return nullptr;
    return &it->second;
}

void ActionMap::Update()
{
    if (!m_active)
        return;

    m_actionItemsLock.LockNow();
    for (rvar item : m_actionItemMap)
    {
        var actionItem = &item.second;

        // We need to make something like this:
        // if(InputManager::IsButton(actionItem->buttonBinds[0]))
        //    actionItem->eventListeners[0].Invoke();
        // But a lot faster, so we are going to need the generic input buffer (but how?)
        // Maybe just go ahead, and make it per-frame input buffer, and let devices and maps
        // do what they want?

        // TODO: Event implementation
        // TODO: State implementation
        // TODO: Axis implementation
    }
    m_actionItemsLock.UnlockNow();
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

    // Add key bind
    if (action)
        action->keyBinds.push_back(key);
}

void ActionMap::AddBind(const char* name, const Button button)
{
    cvar action = GetActionItem(name);

    // Add button bind
    if (action)
        action->buttonBinds.push_back(button);
}

void ActionMap::RemoveBinds(const char* name)
{
    cvar action = GetActionItem(name);
    if (!action)
        return;

    // Clear key and button binds
    action->keyBinds.clear();
    action->buttonBinds.clear();
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
