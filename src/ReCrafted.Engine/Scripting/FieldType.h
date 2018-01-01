// ReCrafted (c) 2016-2018 Always Too Late

#pragma once

#ifndef FIELDTYPE_H
#define FIELDTYPE_H

/// <summary>
/// FieldType
/// </summary>
struct FieldType
{
	enum _enum
	{
		Void = 0x01,
		Bool = 0x02,
		Char = 0x03,
		I1 = 0x04,
		U1 = 0x05,
		I2 = 0x06,
		U2 = 0x07,
		I4 = 0x08,
		U4 = 0x09,
		I8 = 0x0a,
		U8 = 0x0b,
		R4 = 0x0c,
		R8 = 0x0d,
		String = 0x0e,
		Ptr = 0x0f,
		ByRef = 0x10,
		ValueType = 0x11,
		Class = 0x12,
		Var = 0x13,
		Array = 0x14,
		Enum = 0x55,
	};
};

#endif // FIELDTYPE_H
