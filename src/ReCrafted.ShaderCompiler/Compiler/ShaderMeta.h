// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef SHADERMETA_H
#define SHADERMETA_H

// includes
#include <vector>

struct shadermeta_pass_define
{
	char name[32] = {};
};

struct shadermeta_pass_sampler
{
	char name[32] = {};
	char type[32] = {};
};

struct shadermeta_pass
{
	char name[32] = {};

	char vs_method[16] = {};
	char ps_method[16] = {};
	char cs_method[16] = {};

	char vs_profile[5] = {};
	char ps_profile[5] = {};
	char cs_profile[5] = {};

	std::vector<shadermeta_pass_define> defines = {};
	std::vector<shadermeta_pass_sampler> samplers = {};
};

struct shadermeta_cbuffer_field
{
	char name[32] = {};
	char type[16] = {};
};

struct shadermeta_cbuffer
{
	char name[32] = {};
	std::vector<shadermeta_cbuffer_field> fields = {};
};

struct shadermeta
{
public:
	char name[64] = {};
	char desc[2048] = {};

	std::vector<shadermeta_pass> passes = {};
	std::vector<shadermeta_cbuffer> buffers = {};



	// TODO: textures

	// TODO: permutation
};

#endif // SHADERMETA_H
