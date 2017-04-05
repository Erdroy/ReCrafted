// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d.h"

#include <fstream>
#include <vector>
#include <string>

#include "r3d_shader.utils.h"
#include "generators/r3d_shader.d3d11.h"

namespace r3d
{
	std::string process_shader(r3d_shadertype::Enum type, const char* shader_file, std::string section, bool all_platforms, std::vector<std::string>* sources)
	{
		auto has_input = false;
		auto has_output = false;
		std::string includes = {};
		std::string input = {};
		std::string output = {};
		std::vector<std::string> buffers = {};
		std::vector<std::string> buffer_names = {};

		auto sourcecode = section;

		// first pass
		while(true)
		{
			auto found_includes = false;
			for (auto j = 0; j < int(sourcecode.size()); j++)
			{
				auto block_line = compiler_utils::getLine(sourcecode, j);

				// handle #include
				if (compiler_utils::startsWith(block_line, "#include"))
				{
					found_includes = true;

					auto file = compiler_utils::readFromTo(block_line, '"', '"', true);

					// read the file
					auto path = compiler_utils::getRelativePath(shader_file);
					std::ifstream headerStream(path + file);
					std::string header{
						std::istreambuf_iterator<char>(headerStream),
						std::istreambuf_iterator<char>()
					};

					includes += header + "\n";
				}

				j += static_cast<int>(block_line.length());
			}

			// temporary solution: comment-out all includes
			compiler_utils::replaceAll(sourcecode, "#include", "//include");

			if (!found_includes)
				break;
		}

		// add includes at the start
		{
			auto tmp = sourcecode;
			sourcecode = includes + tmp;
		}

		// second pass
		for (auto j = 0; j < int(sourcecode.size()); j++)
		{
			auto block_line = compiler_utils::getLine(sourcecode, j);

			// input block
			if (compiler_utils::startsWith(block_line, "Input"))
			{
				if (has_input)
					throw;

				input = compiler_utils::extractSection(sourcecode, j);
				has_input = true;
				j += static_cast<int>(input.length());
				continue;
			}

			// output block
			if (compiler_utils::startsWith(block_line, "Output"))
			{
				if (has_output)
					throw;

				output = compiler_utils::extractSection(sourcecode, j);
				has_output = true;
				j += static_cast<int>(output.length());
				continue;
			}

			// buffer block
			if (compiler_utils::startsWith(block_line, "Buffer"))
			{
				auto name = compiler_utils::readFromTo(block_line, '(', ')', true);
				buffer_names.push_back(name);

				auto block = compiler_utils::extractSection(sourcecode, j);
				buffers.push_back(block);
				j += static_cast<int>(block.length());
				continue;
			}

			// handle #include
			if (compiler_utils::startsWith(block_line, "#include"))
			{
				auto file = compiler_utils::readFromTo(block_line, '"', '"', true);

			}

			j += static_cast<int>(block_line.length());
		}

		// remove all non-source blocks
		auto section_parsed = sourcecode;
		compiler_utils::removeFromTo(section_parsed, "Input", "}");
		compiler_utils::removeFromTo(section_parsed, "Output", "}");

		if(buffers.size() > 0u)
			compiler_utils::removeFromTo(section_parsed, "Buffer", "}");

		// generate code, inject r3d API and optimize
		// TODO: switch platforms!
		generate_shader_d3d11(type, section_parsed, input, output, buffers, buffer_names);

		return section_parsed;
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

		std::string vs_source = {};
		std::string ps_source = {};

		// read shader sections
		for (auto i = 0; i < source.length(); i++)
		{
			auto line = compiler_utils::getLine(source, i);

			// read sections
			if (compiler_utils::startsWith(line, "VertexShader"))
			{
				auto section = compiler_utils::extractSection(source, i);

				vs_source = process_shader(r3d_shadertype::vertexshader, shader_file, section, all_platforms, &vertexshader_sources);

				i += static_cast<int>(section.length());
				continue;
			}
			if (compiler_utils::startsWith(line, "PixelShader"))
			{
				auto section = compiler_utils::extractSection(source, i);

				ps_source = process_shader(r3d_shadertype::pixelshader, shader_file, section, all_platforms, &pixelshader_sources);

				i += static_cast<int>(section.length());
				continue;
			}
			if (compiler_utils::startsWith(line, "ComputeShader"))
			{
				throw; // TODO: ComputeShader support
			}

			i += static_cast<int>(line.length());
		}
		
		auto vs_compiled = compile_shader_d3d11(r3d_shadertype::vertexshader, vs_source);
		auto ps_compiled = compile_shader_d3d11(r3d_shadertype::pixelshader, ps_source);

		// TODO: compile shaders for all platforms(if(all_platforms), else only for the current one)
		
		// TODO: save - and done!
		
	}

	void load_shader(const char* shader_file, r3d_shader_handle* shader_handle)
	{
		
	}
}
