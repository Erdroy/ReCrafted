// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ENTITY_H
#define ENTITY_H

// includes
#include "ReCrafted.h"
#include "Text.h"
#include "Transform.h"
#include "Scripting/Script.h"

class Entity
{
	friend class EntityPool;

private:
	Transform m_transform = {};
	Text m_name = Text::empty();

	std::vector<Ptr<Script>> m_scripts = {};
	std::vector<Ptr<Entity>> m_children = {};

public:
	void addScript(Ptr<Script> script);
	void removeScript(Ptr<Script> script);

	void addChildren(Ptr<Entity> entity);
	void removeChildren(Ptr<Entity> entity);
};

#endif // ENTITY_H
