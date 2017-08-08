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

	void read(File& file)
	{
		name = file.read_string();
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

	void read(File& file)
	{
		name = file.read_string();
		type = file.read_string();
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

	std::vector<shadermeta_pass_define> defines = {};
	std::vector<shadermeta_pass_sampler> samplers = {};
	
	void write(File& file)
	{
		file.write_string(name);

		file.write_string(vs_method);
		file.write_string(ps_method);
		file.write_string(cs_method);

		file.write_string(vs_profile);
		file.write_string(ps_profile);
		file.write_string(cs_profile);

		file.write(static_cast<int>(defines.size()));
		for (auto & v : defines)
			v.write(file);

		file.write(static_cast<int>(samplers.size()));
		for (auto & v : samplers)
			v.write(file);
	}

	void read(File& file)
	{
		name = file.read_string();

		vs_method = file.read_string();
		ps_method = file.read_string();
		cs_method = file.read_string();

		vs_profile = file.read_string();
		ps_profile = file.read_string();
		cs_profile = file.read_string();

		auto defines_len = file.read<int>();
		defines.resize(defines_len);
		for (auto & v : defines)
			v.read(file);

		auto samplers_len = file.read<int>();
		samplers.resize(samplers_len);
		for (auto & v : samplers)
			v.read(file);
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

	void read(File& file)
	{
		name = file.read_string();
		type = file.read_string();
	}
};

struct shadermeta_cbuffer
{
	std::string name = {};

	std::vector<shadermeta_cbuffer_field> fields = {};

	void write(File& file)
	{
		file.write_string(name);

		file.write(static_cast<int>(fields.size()));

		for (auto & v : fields)
			v.write(file);
	}

	void read(File& file)
	{
		name = file.read_string();

		auto fields_len = file.read<int>();
		fields.resize(fields_len);
		for (auto & v : fields)
			v.read(file);
	}
};

struct shadermeta
{
public:
	std::string name = {};
	std::string desc = {};

	std::vector<shadermeta_pass> passes = {};
	std::vector<shadermeta_cbuffer> buffers = {};

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

	void read(File& file)
	{
		name = file.read_string();
		desc = file.read_string();

		auto passes_len = file.read<int>();
		passes.resize(passes_len);
		for(auto & v : passes)
			v.read(file);

		auto buffers_len = file.read<int>();
		buffers.resize(buffers_len);
		for (auto & v : buffers)
			v.read(file);
	}
};

#endif // SHADERMETA_H
