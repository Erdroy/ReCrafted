// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

// includes
#include "ReCrafted.h"

class Script
{
public:
	void dispose();

public:
	static Ptr<Script> createScript()
	{
		return nullptr;
	}
};

#endif // SCRIPT_H
