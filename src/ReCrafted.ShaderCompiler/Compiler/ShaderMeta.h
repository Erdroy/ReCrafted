// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef SHADERMETA_H
#define SHADERMETA_H

// includes
#include <vector>
#include <string>

struct shadermeta_pass_define
{
	std::string name = {};
};

struct shadermeta_pass_sampler
{
	std::string name = {};
	std::string type = {};
};

struct shadermeta_pass
{
	std::string name = {};

	std::string vs_method = {};
	std::string ps_method = {};
	std::string cs_method = {};

	std::string vs_profile = {};
	std::string ps_profile = {};
	std::string cs_profile = {};

	std::vector<shadermeta_pass_define> defines;
	std::vector<shadermeta_pass_sampler> samplers;
};

struct shadermeta_cbuffer_field
{
	std::string name = {};
	std::string type = {};
};

struct shadermeta_cbuffer
{
	std::string name = {};

	std::vector<shadermeta_cbuffer_field> fields;
};

struct shadermeta
{
public:
	std::string name = {};
	std::string desc = {};

	std::vector<shadermeta_pass> passes;
	std::vector<shadermeta_cbuffer> buffers;

	// TODO: textures

	// TODO: permutation
};

#endif // SHADERMETA_H
