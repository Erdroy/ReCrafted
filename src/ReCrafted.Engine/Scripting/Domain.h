// ReCrafted (c) 2016-2018 Always Too Late

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

	Array<Ref<Assembly>> m_loadedAssemblies = {};

public:
	/// <summary>
	/// Loads assembly using given fileName.
	/// </summary>
	Ref<Assembly> loadAssembly(const char* fileName);

	/// <summary>
	/// Cleanups the domain.
	/// </summary>
	void cleanup();

	MonoDomain* getMono() const;

public:
	/// <summary>
	/// Creates Domain from MonoDomain.
	/// </summary>
	static Ref<Domain> create(MonoDomain* monoDomain);

	/// <summary>
	/// Creates domain of given name.
	/// </summary>
	static Ref<Domain> create(const char* name, Ref<Domain> parent);

	/// <summary>
	/// Creates root domain.
	/// </summary>
	static Ref<Domain> createRoot();

public:
	static Ref<Domain> Root;
};

#endif // DOMAIN_H
