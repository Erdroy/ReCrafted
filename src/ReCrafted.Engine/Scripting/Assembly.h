// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef ASSEMBLY_H
#define ASSEMBLY_H

// includes
#include "Utils/Types.h"

class Assembly
{
	friend class ScriptingEngine;
	friend class Domain;

private:
	MonoAssembly* m_assembly = nullptr;

public:
	
};

#endif // ASSEMBLY_H
