// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef PARSER_H
#define PARSER_H

// includes
#include "ShaderMeta.h"

class Parser
{
public:
	/**
	 * \brief Parses given HLSL (ReCrafted subset) code into shader meta data.
	 * \param code The code bytes.
	 * \param length The code byte length.
	 * \param meta The output shader meta.
	 * \return Returns true when succeeded.
	 */
	static bool parse(const char* code, size_t length, shadermeta* meta);
};

#endif // PARSER_H
