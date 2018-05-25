// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef ENTITY_H
#define ENTITY_H

// includes
#include "Text.h"
#include "Core/Guid.h"
#include "Scripting/Script.h"
#include "Scripting/Object.h"
#include "Core/Math/Math.h"
#include "Core/Containers/Array.h"

class Entity : public Object
{
SCRIPTING_API_IMPL()

private:
    friend class EntityPool;
    friend class GameMain;

public:
    Array<Ref<Script>> scripts = {};
    Array<Ref<Entity>> children = {};

    Vector3 position = {};
    Vector3 rotation = {};

    Guid guid = {};
    Text name = {};

    Entity* parent = nullptr;

public:
    void AddScript(Ref<Script> script);
    void RemoveScript(Ref<Script> script);

    void AddChildren(Ref<Entity> entity);
    void RemoveChildren(Ref<Entity> entity);

public:
    void Update();
    void Simulate();

public:
    void OnDestroy() override;
};

#endif // ENTITY_H
