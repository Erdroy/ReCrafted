// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Method.h"

void Method::invoke() const
{
	MonoObject* exception = nullptr;
	mono_runtime_invoke(m_method, m_object, nullptr, &exception);

	if(exception)
	{
		// TODO: raise exception to C#
	}
}
