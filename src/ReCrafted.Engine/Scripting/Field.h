// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef FIELD_H
#define FIELD_H

// includes
#include "Mono.h"
#include "FieldType.h"

class Field
{
	friend class Object;

private:
	MonoObject* m_object = nullptr;
	MonoClassField* m_field = nullptr;

public:
	/// <summary>
	/// Sets value.
	/// </summary>
	void setValue(void* value) const;

	/// <summary>
	/// Gets value.
	/// </summary>
	void getValue(void* value) const;

	/// <summary>
	/// Gets field type.
	/// </summary>
	FieldType::_enum getType() const;
};
#endif // FIELD_H
