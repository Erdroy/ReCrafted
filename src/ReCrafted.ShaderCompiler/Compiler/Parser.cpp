// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Parser.h"
#include <cstring>
#include <cstdio>
#include <ctype.h>


namespace helpers
{
	bool starts_with(const char* pre, const char* str)
	{
		auto lenpre = strlen(pre);
		return strlen(str) < lenpre ? false : strncmp(pre, str, lenpre) == 0;
	}

	// http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
	char* trim(char* str)
	{
		char* end;

		// Trim leading space
		while (isspace(static_cast<unsigned char>(*str))) str++;

		if (*str == 0)  // All spaces?
			return str;

		// Trim trailing space
		end = str + strlen(str) - 1;
		while (end > str && isspace(static_cast<unsigned char>(*end))) end--;

		// Write new null terminator
		*(end + 1) = 0;

		return str;
	}

	bool strcontains(char* str, char ch)
	{
		auto len = strlen(str);
		for (auto i = 0u; i < len; i++)
		{
			if (str[i] == ch)
				return true;
		}

		return false;
	}

	int readfromto(char* str, std::string& buffer, char start, char end, bool ignore_spaces = false)
	{
		auto len = strlen(str);
		auto read = false;

		for (auto i = 0u; i < len; i++)
		{
			if (read)
			{
				if (str[i] == end)
				{
					// end
					return 0; // 0 - ok
				}

				if (ignore_spaces && isspace(str[i]))
					continue;

				buffer.push_back(str[i]);
			}

			if (str[i] == start)
			{
				// start
				read = true;
			}
		}

		return !read ? 1 : 2; // 1 -  no start, 2 - no end
	}

	bool readto(char* str, char* buffer, char to, bool ignore_spaces = false)
	{
		auto len = strlen(str);
		auto charid = 0;

		for (auto i = 0u; i < len; i++)
		{
			if (str[i] == to)
			{
				// end
				return true;
			}

			if (ignore_spaces && isspace(str[i]))
				continue;

			buffer[charid] = str[i];
			charid++;
		}

		return false;
	}

	bool readtospace(char* str, std::string& buffer)
	{
		auto len = strlen(str);

		for (auto i = 0u; i < len; i++)
		{
			if (isspace(str[i]))
			{
				// end
				return true;
			}

			buffer.push_back(str[i]);
		}

		return false;
	}
}
using namespace helpers;


#define MLINE_CBUFFER		1
#define MLINE_PASS			2

bool g_ignoreline = false;
bool g_readmline = false;
int g_mlinetype = 0;


// parsers

bool parse_metaname(char* line, int linenum, shadermeta* meta)
{
	// read from " to "
	auto result = readfromto(line, meta->name, '"', '"');

	// throw error when start/end is missing
	if (result != 0)
	{
		if (result == 1)
		{
			printf("Error: No meta string start character \" was found at line %d.\n", linenum);
			return false;
		}

		printf("Error: No meta string end character \" was found at line %d.\n", linenum);
		return false;
	}

	return true;
}

bool parse_metadesc(char* line, int linenum, shadermeta* meta)
{
	// read from " to "
	auto result = readfromto(line, meta->desc, '"', '"');

	// throw error when start/end is missing
	if (result != 0)
	{
		if (result == 1)
		{
			printf("Error: No meta string start character \" was found at line %d.\n", linenum);
			return false;
		}

		printf("Error: No meta string end character \" was found at line %d.\n", linenum);
		return false;
	}

	return true;
}

bool parse_pass(char* line, int linenum, shadermeta* meta)
{
	shadermeta_pass pass = {};

	// read from space to space/newline
	auto result = readfromto(line, pass.name, ' ', '{', true);

	if (result != 0)
	{
		printf("Error: Invalid pass definition at line %d.\n", linenum);
		return false;
	}

	// push new with name
	meta->passes.push_back(pass);

	return true;
}

bool parse_cbuffer(char* line, int linenum, shadermeta* meta)
{
	shadermeta_cbuffer buffer = {};

	// read from space to :
	auto result = readfromto(line, buffer.name, ' ', ':', true);

	if (result != 0)
	{
		printf("Error: Invalid constant buffer definition at line %d.\n", linenum);
		return false;
	}

	// push new with name
	meta->buffers.push_back(buffer);

	return true;
}

bool parse_passline(char* line, int linenum, shadermeta* meta)
{
	if (starts_with("VertexShader", line))
	{
		auto pass = &meta->passes[meta->passes.size() - 1];

		auto result = readfromto(line, pass->vs_method, '(', ',', true);

		if (result != 0)
		{
			printf("Error: Invalid pass vertex shader definition at line %d.\n", linenum);
			return false;
		}

		result = readfromto(line, pass->vs_profile, ',', ')', true);

		if (result != 0)
		{
			printf("Error: Invalid pass vertex shader definition at line %d.\n", linenum);
			return false;
		}

		return true;
	}
	if (starts_with("PixelShader", line))
	{
		auto pass = &meta->passes[meta->passes.size() - 1];

		auto result = readfromto(line, pass->ps_method, '(', ',', true);

		if (result != 0)
		{
			printf("Error: Invalid pass vertex shader definition at line %d.\n", linenum);
			return false;
		}

		result = readfromto(line, pass->ps_profile, ',', ')', true);

		if (result != 0)
		{
			printf("Error: Invalid pass vertex shader definition at line %d.\n", linenum);
			return false;
		}

		return true;
	}
	if (starts_with("ComputeShader", line))
	{
		auto pass = &meta->passes[meta->passes.size() - 1];

		auto result = readfromto(line, pass->cs_method, '(', ',', true);

		if (result != 0)
		{
			printf("Error: Invalid pass vertex shader definition at line %d.\n", linenum);
			return false;
		}

		result = readfromto(line, pass->cs_profile, ',', ')', true);

		if (result != 0)
		{
			printf("Error: Invalid pass vertex shader definition at line %d.\n", linenum);
			return false;
		}

		return true;
	}
	if (starts_with("SamplerState", line))
	{
		shadermeta_pass_sampler sampler = {};

		auto result = readfromto(line, sampler.name, '(', ',', true);

		if (result != 0)
		{
			printf("Error: Invalid pass sampler state definition at line %d.\n", linenum);
			return false;
		}

		result = readfromto(line, sampler.type, ',', ')', true);

		if (result != 0)
		{
			printf("Error: Invalid pass sampler state definition at line %d.\n", linenum);
			return false;
		}

		// check pass
		if (strcmp(sampler.type.c_str(), "PointClamped") != 0 &&
			strcmp(sampler.type.c_str(), "PointMirror") != 0 &&
			strcmp(sampler.type.c_str(), "PointWarp") != 0 &&

			strcmp(sampler.type.c_str(), "LinearClamped") != 0 &&
			strcmp(sampler.type.c_str(), "LinearMirror") != 0 &&
			strcmp(sampler.type.c_str(), "LinearWarp") != 0 &&

			strcmp(sampler.type.c_str(), "AnisoClamped") != 0 &&
			strcmp(sampler.type.c_str(), "AnisoMirror") != 0 &&
			strcmp(sampler.type.c_str(), "AnisoWarp") != 0)
		{
			printf("Error: Invalid sampler type at line %d.\n", linenum);
			return false;
		}

		meta->passes[meta->passes.size() - 1].samplers.push_back(sampler);

		return true;
	}
	if (starts_with("Define", line))
	{
		shadermeta_pass_define define = {};

		// read from space to :
		auto result = readfromto(line, define.name, '(', ')', true);

		if (result != 0)
		{
			printf("Error: Invalid constant buffer definition at line %d.\n", linenum);
			return false;
		}

		// push new define
		meta->passes[meta->passes.size() - 1].defines.push_back(define);

		return true;
	}

	return true;
}

bool parse_cbufferline(char* line, int linenum, shadermeta* meta)
{
	shadermeta_cbuffer_field field = {};

	if (!readtospace(line, field.type))
	{
		printf("Error: Invalid cbuffer definition at line %d.\n", linenum);
		return false;
	}

	if (readfromto(line, field.name, ' ', ';', true) != 0)
	{
		printf("Error: Invalid cbuffer definition at line %d.\n", linenum);
		return false;
	}

	meta->buffers[meta->buffers.size() - 1].fields.push_back(field);
	return true;
}



// ----parser-main----

bool process_line(char* line, int linenum, shadermeta* meta)
{
	if (starts_with("#name", line))
	{
		g_ignoreline = true;
		return parse_metaname(line, linenum, meta);
	}
	if (starts_with("#desc", line))
	{
		g_ignoreline = true;
		return parse_metadesc(line, linenum, meta);
	}
	if (starts_with("pass", line))
	{
		g_ignoreline = true;
		g_readmline = true;
		g_mlinetype = MLINE_PASS;
		return parse_pass(line, linenum, meta);
	}
	if (starts_with("cbuffer", line))
	{
		g_readmline = true;
		g_mlinetype = MLINE_CBUFFER;
		return parse_cbuffer(line, linenum, meta);
	}

	// TODO: samplers and textures support
	// Samplers should be 'built-in', so user won't need to set them

	if (g_readmline)
	{
		if (g_mlinetype != MLINE_CBUFFER)
			g_ignoreline = true;

		if (strcontains(line, '}'))
		{
			g_mlinetype = 0;
			g_readmline = false;
			return true;
		}

		switch (g_mlinetype)
		{
		case MLINE_CBUFFER:
		{
			if (!parse_cbufferline(line, linenum, meta))
				return false;
		}
		break;
		case MLINE_PASS:
		{
			if (!parse_passline(line, linenum, meta))
				return false;
		}
		break;
		default:
			printf("Error: Multiline parsing error at line %d.", linenum);
			return false;
		}
	}

	return true;
}

bool Parser::parse(const char* code, size_t length, shadermeta* meta)
{
	auto codelength = static_cast<int>(length);

	// create line text buffer
	auto linenum = 1;
	auto linepos = 0;
	auto linestart = 0;
	auto mcomment = false;
	auto scomment = false;
	char line[4096];
	memset(line, 0, 4096);

	// read line by line
	for (auto i = 0; i < codelength; i++)
	{
		auto character = code[i];

		if (character == '/' && code[i + 1] == '*')
		{
			// comment!
			mcomment = true;
		}

		if (character == '*' && code[i + 1] == '/')
		{
			// end of comment
			mcomment = false;

			// ignore next character '/'
			i++;
			continue;
		}

		if (character == '/' && code[i + 1] == '/')
		{
			// comment!
			scomment = true;

			// ignore next character '/'
			i++;
			continue;
		}

		if (character == '\n' || i + 1 == codelength) // if new line or EOF
		{
			// trim line start
			auto trimline = trim(line);

			// process line
			if (!scomment && !mcomment)
			{
				if (!process_line(trimline, linenum, meta))
					return false;
			}

			// remove this line from code if needed
			if (g_ignoreline)
			{
				// set the line to spaces
				auto ptr = const_cast<char*>(code) + linestart;
				memset(ptr, ' ', i - linestart);
				g_ignoreline = false;
			}

			scomment = false;

			// reset
			linestart = i + 1;
			linepos = 0;
			memset(line, 0, 4096);
			linenum++;
		}
		else if (!mcomment && !scomment)
		{
			line[linepos] = character;
			linepos++;

			if (linepos >= 4096)
			{
				printf("Error: Line %d is too long.", linenum);
				return false;
			}
		}
	}

	return true;
}