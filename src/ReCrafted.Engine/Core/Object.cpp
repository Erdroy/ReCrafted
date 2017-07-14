// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Object.h"

void Object::onFinalize()
{
	// cleanup
	m_ptr = nullptr;
	delete this;
}

Object::Object(void* ptr) : m_ptr(ptr)
{
}
