// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d.h"

#include <fstream>
#include <vector>
#include <string>

#include "r3d_shader.utils.h"
#include "generators/r3d_shader.d3d11.h"

namespace r3d
{
	void parse_shader(int source_length, std::string section, bool all_platforms, std::vector<std::string>* sources)
	{
		auto has_input = false;
		auto has_output = false;
		std::string input = {};
		std::string output = {};
		std::vector<std::string> buffers = {};
		std::vector<std::string> buffer_names = {};

		auto section_parsed = section;

		for (auto j = 0; j < source_length; j++)
		{
			auto block_line = compiler_utils::getLine(section, j);

			// input block
			if (compiler_utils::startsWith(block_line, "Input"))
			{
				if (has_input)
					throw;

				input = compiler_utils::extractSection(section, j);
				has_input = true;
				j += static_cast<int>(input.length());
				continue;
			}

			// output block
			if (compiler_utils::startsWith(block_line, "Output"))
			{
				if (has_output)
					throw;

				output = compiler_utils::extractSection(section, j);
				has_output = true;
				j += static_cast<int>(output.length());
				continue;
			}

			// buffer block
			if (compiler_utils::startsWith(block_line, "Buffer"))
			{
				auto name = compiler_utils::readFromTo(block_line, '(', ')', true);
				buffer_names.push_back(name);

				auto block = compiler_utils::extractSection(section, j); 
				buffers.push_back(block);
				j += static_cast<int>(block.length());
				continue;
			}

			j += static_cast<int>(block_line.length());
		}

		// remove all non-source blocks
		compiler_utils::removeFromTo(section_parsed, "Input", "}");
		compiler_utils::removeFromTo(section_parsed, "Output", "}");
		compiler_utils::removeFromTo(section_parsed, "Buffer", "}");

		// generate code, inject r3d API and optimize
		
		{
			generate_d3d11(section_parsed, input, output, buffers, buffer_names);
		}
	}

	void compile_shader(const char* shader_file, const char* output_file, bool all_platforms)
	{
		// load file
		std::ifstream shaderfile(shader_file);
		std::string source{
			std::istreambuf_iterator<char>(shaderfile),
			std::istreambuf_iterator<char>()
		};

		// remove comments
		source = compiler_utils::removeComments(source);

		// remove some tabulators, caret-something-shit.
		compiler_utils::replaceAll(source, "\t", "");
		compiler_utils::replaceAll(source, "\r", "");

		std::vector<std::string> vertexshader_sources = {};
		std::vector<std::string> pixelshader_sources = {};

		// read shader sections
		for (auto i = 0; i < source.length(); i++)
		{
			auto line = compiler_utils::getLine(source, i);

			// read sections
			if (compiler_utils::startsWith(line, "VertexShader"))
			{
				auto section = compiler_utils::extractSection(source, i);

				parse_shader(int(section.length()), section, all_platforms, &vertexshader_sources);

				i += static_cast<int>(section.length());
				continue;
			}
			if (compiler_utils::startsWith(line, "PixelShader"))
			{
				auto section = compiler_utils::extractSection(source, i);

				parse_shader(int(section.length()), section, all_platforms, &pixelshader_sources);

				i += static_cast<int>(section.length());
				continue;
			}
			if (compiler_utils::startsWith(line, "ComputeShader"))
			{
				throw; // TODO: ComputeShader support
			}

			i += static_cast<int>(line.length());
		}

		// TODO: compile shaders for all platforms(if(all_platforms), else only for the current one)
		
		// TODO: save - and done!
		
	}

	void load_shader(const char* shader_file, r3d_shader_handle* shader_handle)
	{
		
	}
}
