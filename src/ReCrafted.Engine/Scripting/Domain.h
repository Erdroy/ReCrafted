// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef DOMAIN_H
#define DOMAIN_H

// includes
#include "Core/Types.h"
#include "Assembly.h"
#include "Mono.h"
#include "Core/Containers/Array.h"

#include <vector>

class Domain
{
	friend class ScriptingEngine;
	friend class Class;

private:
	MonoDomain* m_domain = nullptr;

	Array<Ptr<Assembly>> m_loadedAssemblies = {};

public:
	/// <summary>
	/// Loads assembly using given fileName.
	/// </summary>
	Ptr<Assembly> loadAssembly(const char* fileName);

	/// <summary>
	/// Cleanups the domain.
	/// </summary>
	void cleanup();

	MonoDomain* getMono() const;

public:
	/// <summary>
	/// Creates Domain from MonoDomain.
	/// </summary>
	static Ptr<Domain> create(MonoDomain* monoDomain);

	/// <summary>
	/// Creates domain of given name.
	/// </summary>
	static Ptr<Domain> create(const char* name, Ptr<Domain> parent);

	/// <summary>
	/// Creates root domain.
	/// </summary>
	static Ptr<Domain> createRoot();

public:
	static Ptr<Domain> Root;
};

#endif // DOMAIN_H
