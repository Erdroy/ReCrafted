// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef SCRIPT_H
#define SCRIPT_H

// includes
#include "Object.h"

class Script : public Object
{
    API_DEF

public:
	void dispose();

public:
	static Ptr<Script> createScript()
	{
		return nullptr;
	}
};

#endif // SCRIPT_H
