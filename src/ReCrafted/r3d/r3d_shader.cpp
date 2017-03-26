// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d.h"

#include <fstream>

namespace r3d
{
	class compiler_utils 
	{
	public:
		static bool replace(std::string& str, const std::string& from, const std::string& to) {
			size_t start_pos = str.find(from);
			if (start_pos == std::string::npos)
				return false;
			str.replace(start_pos, from.length(), to);
			return true;
		}

		static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
			if (from.empty())
				return;
			size_t start_pos = 0;
			while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
				str.replace(start_pos, from.length(), to);
				start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
			}
		}

		static std::string removeComments(std::string code)
		{
			std::string preprocessedCode;

			auto comment = false;
			auto multiComment = false;

			for (auto i = 0; i < code.length(); i++)
			{
				if (comment == true && code[i] == '\n')
				{
					comment = false;
				}
				else if (multiComment == true && code[i] == '*' && code[i + 1] == '/')
				{
					multiComment = false, i++;
				}
				else if (comment || multiComment)
				{
					// Ignore this character.
					//continue;
				}
				else if (code[i] == '/' && code[i + 1] == '/')
				{
					comment = true;
					i++;
				}
				else if (code[i] == '/' && code[i + 1] == '*')
				{
					multiComment = true;
					i++;
				}
				else
				{
					preprocessedCode += code[i];
				}
			}
			return preprocessedCode;
		}

		static std::string getLine(std::string source, int i)
		{
			std::string ret;
			auto ignorespace = true;

			for (auto j = i; j < source.length(); j++)
			{
				auto ch = source[j];

				if (ignorespace && ch == ' ')
					continue;

				// stop ignoring spaces
				ignorespace = false;

				if (ch == '\n')
				{
					break;
				}

				ret += ch;
			}

			return ret;
		}

		static bool startsWith(std::string& str, const char* text)
		{
			auto textlen = strlen(text) - 1;
			auto strtext = str.c_str();

			for (auto i = 0; i < textlen; i++)
			{
				if (strtext[i] != text[i])
					return false;
			}

			return true;
		}

		static std::string extractSection(std::string& source, int i)
		{
			// start reading into section when found '{' and end on last '}'

			std::string section = {};
			auto bracketCount = 0;
			auto read = false;
			for (auto j = i; j < source.length(); j++)
			{
				if (source[j] == '{')
				{
					if (bracketCount == 0)
					{
						// start
						read = true;
						bracketCount++;
						continue;
					}

					// count up
					bracketCount++;
				}
				else if (source[j] == '}')
				{
					// count down
					bracketCount--;

					if (bracketCount == 0)
					{
						// end
						break;
					}
				}

				if (read)
				{
					section += source[j];
				}
			}

			if (bracketCount != 0)
				throw; // TODO: handle errors

			return section;
		}
	};


	void compile_shader_d3d11(const char* shader_source)
	{
		// ???
	}

	void compile_shader(const char* shader_file, const char* output_file)
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

		// read shader sections
		for (auto i = 0; i < source.length(); i++)
		{
			auto line = compiler_utils::getLine(source, i);

			// read sections
			if (compiler_utils::startsWith(line, "VertexShader"))
			{
				auto section = compiler_utils::extractSection(source, i);

				// TODO: read Input, Output and Buffer [?], construct needed things and remove from source

				i += static_cast<int>(section.length());
				continue;
			}
			if (compiler_utils::startsWith(line, "PixelShader"))
			{
				auto section = compiler_utils::extractSection(source, i);

				// TODO: read Input, Output and Buffer [?], construct needed things and remove from source

				i += static_cast<int>(section.length());
				continue;
			}
			if (compiler_utils::startsWith(line, "ComputeShader"))
			{
				throw; // TODO: ComputeShader support
			}

			i += static_cast<int>(line.length());
		}

		// TODO: parse and preprocess shader sections

		// compile shaders
		auto apitype = r3d::get_apitype();

		switch (apitype)
		{
		case r3d_apitype::d3d11:
			compile_shader_d3d11(shader_file);
			return;
		case r3d_apitype::d3d12: throw;
		case r3d_apitype::opengl4: throw;
		case r3d_apitype::vulkan: throw;
		default: break;
		}

		// create shaders

		// done!
		
	}


	void load_shader(const char* shader_file, r3d_shader_handle* shader_handle)
	{
		
	}
}
