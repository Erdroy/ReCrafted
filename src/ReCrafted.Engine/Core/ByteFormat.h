// ReCrafted © 2016-2017 Always Too Late

#pragma once

#ifndef BYTEFORMAT_H
#define BYTEFORMAT_H

// includes
#include "ReCrafted.h"

struct ByteFormat
{
public:
	enum _enum
	{
		B,
		KB,
		MB,
		GB,

		Count
	};

	/**
	 * \brief Formats given value to the higher unit.
	 * \param value The value.
	 * \param outValue The formated value.
	 * \return The unit format.
	 */
	FORCEINLINE static _enum Format(int64_t value, float* outValue)
	{
		var finalValue = static_cast<float>(value);

		for(var i = 0; i < Count; i ++)
		{
			var currentValue = finalValue / 1000.0f;

			if(currentValue < 0.01f)
			{
				*outValue = finalValue;
				return static_cast<_enum>(i);
			}

			finalValue = currentValue;
		}

		return B;
	}

	FORCEINLINE static wchar_t* ToString(_enum format)
	{
		switch(format)
		{
		case B:
			return reinterpret_cast<wchar_t*>(TEXT_CHARS("B"));
		case KB:
			return reinterpret_cast<wchar_t*>(TEXT_CHARS("KB"));
		case MB:
			return reinterpret_cast<wchar_t*>(TEXT_CHARS("MB"));
		case GB:
			return reinterpret_cast<wchar_t*>(TEXT_CHARS("GB"));
		case Count:
		default:
			return reinterpret_cast<wchar_t*>(TEXT_CHARS("B"));
		}
	}
};

#endif // BYTEFORMAT_H
