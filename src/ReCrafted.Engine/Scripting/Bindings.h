// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef BINDINGS_H
#define BINDINGS_H

// includes
#include "Object.h"

typedef Ptr<Object>(*objectinstancer)(void);

class Bindings
{
public:
	static void bind();

	static void bindObject(char* typePtr, objectinstancer method);

	static Ptr<Object> instantiate(char* typePtr);

	static void shutdown();
};

#endif // BINDINGS_H
