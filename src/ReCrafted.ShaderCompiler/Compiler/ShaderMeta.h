// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#ifndef SHADERMETA_H
#define SHADERMETA_H

#define SHADER_META_MAX_PASS_DEFINES 32
#define SHADER_META_MAX_PASSES 8
#define SHADER_META_MAX_SAMPLERS 16
#define SHADER_META_MAX_BUFFERS 8
#define SHADER_META_MAX_BUFFER_FIELDS 32

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

	unsigned int defines_count = 0u;
	shadermeta_pass_define defines[SHADER_META_MAX_PASS_DEFINES];

	unsigned int samplers_count = 0u;
	shadermeta_pass_sampler samplers[SHADER_META_MAX_SAMPLERS];

public:
	void add_define(shadermeta_pass_define define)
	{
		defines[defines_count] = define;
		defines_count++;
	}

	void add_sampler(shadermeta_pass_sampler define)
	{
		samplers[samplers_count] = define;
		samplers_count++;
	}
};

struct shadermeta_cbuffer_field
{
	char name[32] = {};
	char type[32] = {};
};

struct shadermeta_cbuffer
{
	char name[32] = {};

	unsigned int fields_count = 0u;
	shadermeta_cbuffer_field fields[SHADER_META_MAX_BUFFER_FIELDS];

public:
	void add_field(shadermeta_cbuffer_field field)
	{
		fields[fields_count] = field;
		fields_count++;
	}
};

struct shadermeta
{
public:
	char name[64] = {};
	char desc[512] = {};

	unsigned int passes_count = 0u;
	shadermeta_pass passes[SHADER_META_MAX_PASSES];

	unsigned int buffers_count = 0u;
	shadermeta_cbuffer buffers[SHADER_META_MAX_BUFFERS];

	// TODO: textures

	// TODO: permutation

public:
	void add_pass(shadermeta_pass pass)
	{
		passes[passes_count] = pass;
		passes_count++;
	}

	void add_buffer(shadermeta_cbuffer buffer)
	{
		buffers[buffers_count] = buffer;
		buffers_count++;
	}

public:
	static shadermeta fromMemory(unsigned char* memory_ptr)
	{

		// read from memory

	}
};

#endif // SHADERMETA_H
