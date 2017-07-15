// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef CLASS_H
#define CLASS_H

// includes
#include "Utils/Types.h"
#include "Object.h"
#include "Mono.h"

class Class
{
	friend class ScriptingEngine;
	friend class Assembly;

private:
	Assembly* m_assembly = nullptr;
	MonoClass* m_class = nullptr;

public:
	/// <summary>
	/// Creates instance of this class.
	/// </summary>
	Ptr<Object> createInstance(bool isObject = true) const;
};

#endif // CLASS_H
