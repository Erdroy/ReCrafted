// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ACTIONMAP_H
#define ACTIONMAP_H

#include "ReCrafted.h"
#include "Scripting/Object.h"

class ActionMap : public Object
{
    friend class InputManager;

private:
    SCRIPTING_API_IMPL();

private:
    std::string m_name;

private:
    void SetName(const std::string& name)
    {
        m_name = name;
    }

    std::string& GetName()
    {
        return m_name;
    }

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


public:
    ActionMap& operator=(const ActionMap& other) = delete;
};

#endif // ACTIONMAP_H
