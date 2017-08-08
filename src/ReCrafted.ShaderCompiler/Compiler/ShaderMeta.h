// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef SHADERMETA_H
#define SHADERMETA_H

// includes
#include <vector>
#include <string>

#include "File.h"

struct shadermeta_pass_define
{
	std::string name = {};

	void write(File& file)
	{
		file.write_string(name);
	}
};

struct shadermeta_pass_sampler
{
	std::string name = {};
	std::string type = {};

	void write(File& file)
	{
		file.write_string(name);
		file.write_string(type);
	}
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
	
	void write(File& file)
	{
		file.write_string(name);

		file.write_string(vs_method);
		file.write_string(ps_method);
		file.write_string(cs_method);

		file.write_string(vs_profile);
		file.write_string(ps_profile);
		file.write_string(cs_profile);

		for (auto & v : defines)
			v.write(file);

		for (auto & v : samplers)
			v.write(file);
	}
};

struct shadermeta_cbuffer_field
{
	std::string name = {};
	std::string type = {};

	void write(File& file)
	{
		file.write_string(name);
		file.write_string(type);
	}
};

struct shadermeta_cbuffer
{
	std::string name = {};

	std::vector<shadermeta_cbuffer_field> fields;

	void write(File& file)
	{
		file.write_string(name);

		file.write(static_cast<int>(fields.size()));

		for (auto & v : fields)
			v.write(file);
	}
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
public:
	void write(File& file)
	{
		file.write_string(name);
		file.write_string(desc);

		file.write(static_cast<int>(passes.size()));
		for(auto & v : passes)
			v.write(file);

		file.write(static_cast<int>(buffers.size()));
		for (auto & v : buffers)
			v.write(file);
	}
};

#endif // SHADERMETA_H
