// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef OBJECT_H
#define OBJECT_H

// includes
#include "Common/ReCraftedAPI.h"

class Object
{
	API_DEF

private:
	void* m_ptr = nullptr; // mono object pointer

private:
	Object() {}

private:
	void onFinalize();

public:
	explicit Object(void* ptr);

public:

public:

};

#endif // OBJECT_H
