// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

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

private:
	std::vector<Ptr<Script>> m_scripts = {};
	std::vector<Ptr<Entity>> m_children = {};

public:
    Vector3 position = {};
    Vector3 rotation = {};

    Guid guid = {};
    Text name = {};

public:
	void addScript(Ptr<Script> script);
	void removeScript(Ptr<Script> script);

	void addChildren(Ptr<Entity> entity);
	void removeChildren(Ptr<Entity> entity);
};

#endif // ENTITY_H
