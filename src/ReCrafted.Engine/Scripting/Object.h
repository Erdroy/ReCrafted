// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef OBJECT_H
#define OBJECT_H

// includes
#include "Common/ReCraftedAPI.h"
#include "Method.h"
#include "Field.h"
#include "Mono.h"
#include "Utils/Types.h"

class Object
{
	friend class Class;
	API_DEF

private:
	MonoObject* m_object = nullptr;
	MonoClass* m_class = nullptr;

private:
	void onFinalize();

public:
	/// <summary>
	/// Finds method using given name.
	/// </summary>
	Ptr<Method> findMethod(const char* methodName) const;

	/// <summary>
	/// Finds field using given name.
	/// </summary>
	Ptr<Field> findField(const char* fieldName) const;
};

#endif // OBJECT_H
