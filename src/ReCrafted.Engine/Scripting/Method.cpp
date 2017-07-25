// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Method.h"
#include "Assembly.h"

void Method::invoke() const
{
	mono_runtime_invoke(m_method, m_object, nullptr, nullptr);
}
