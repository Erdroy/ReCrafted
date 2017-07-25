// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef CLASS_H
#define CLASS_H

// includes
#include "Mono.h"

class Class
{
	friend class ScriptingEngine;
	friend class Assembly;
	friend class Object;
	friend class Method;

private:
	Assembly* m_assembly = nullptr;
	MonoClass* m_class = nullptr;

public:
	/// <summary>
	/// Gets mono type.
	/// </summary>
	char* getType() const;
};

#endif // CLASS_H
