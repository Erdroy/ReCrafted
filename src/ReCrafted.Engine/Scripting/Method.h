// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef METHOD_H
#define METHOD_H

// includes
#include "Mono.h"

class Method
{
	friend class Object;

private:
	MonoObject* m_object = nullptr;
	MonoMethod* m_method = nullptr;

public:
	/// <summary>
	/// Invokes the method.
	/// </summary>
	void invoke() const;
};

#endif // METHOD_H
