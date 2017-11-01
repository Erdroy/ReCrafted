// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef ENTITY_H
#define ENTITY_H

// includes
#include "Text.h"
#include "Core/Guid.h"
#include "Scripting/Script.h"
#include "Scripting/Object.h"
#include "Core/Math/Math.h"

class Entity : public Object
{
    API_DEF

	friend class EntityPool;
    friend class GameMain;

public:
    std::vector<Ptr<Script>> scripts = {};
    std::vector<Ptr<Entity>> children = {};

    Vector3 position = {};
    Vector3 rotation = {};

    Guid guid = {};
    Text name = {};

    Entity* parent = nullptr;

public:
	void addScript(Ptr<Script> script);
	void removeScript(Ptr<Script> script);

	void addChildren(Ptr<Entity> entity);
	void removeChildren(Ptr<Entity> entity);

public:
    void update();
    void simulate();

public:
    void onDestroy() override;
};

#endif // ENTITY_H
