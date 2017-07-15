// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ASSEMBLY_H
#define ASSEMBLY_H

// includes
#include "Utils/Types.h"
#include "Class.h"
#include "Domain.h"
#include "Mono.h"

class Assembly
{
	friend class ScriptingEngine;
	friend class Domain;
	friend class Class;

private:
	MonoDomain* m_domain = nullptr;

	MonoAssembly* m_assembly = nullptr;
	MonoImage* m_image = nullptr;

public:
	/// <summary>
	/// Finds class using namespace and class name.
	/// </summary>
	Ptr<Class> findClass(const char* class_namespace, const char* class_name);
};

#endif // ASSEMBLY_H
