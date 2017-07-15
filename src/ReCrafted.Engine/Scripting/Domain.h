// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

// includes
#include "Utils/Types.h"
#include "ScriptingEngine.h"
#include "Assembly.h"

class Domain
{
	friend class ScriptingEngine;

private:
	MonoDomain* m_domain = nullptr;

public:
	/// <summary>
	/// Loads assembly using given fileName.
	/// </summary>
	Ptr<Assembly> loadAssembly(const char* fileName);

	/// <summary>
	/// Cleanups the domain.
	/// </summary>
	void cleanup();

public:
	/// <summary>
	/// Creates Domain from MonoDomain.
	/// </summary>
	static Ptr<Domain> create(MonoDomain* monoDomain);

	/// <summary>
	/// Creates domain of given name.
	/// </summary>
	static Ptr<Domain> create(const char* name, Ptr<Domain> parent);
};

#endif // DOMAIN_H
