// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#include "r3d.h"

#include <fstream>
#include <vector>

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
		
		static void removeFromTo(std::string& str, std::string fromStr, std::string toStr)
		{
			while (true)
			{
				auto from = -1;
				auto to = 0;

				for (auto i = 0; i < str.length(); i++)
				{
					auto startFound = true;
					for (auto j = i; j < i + fromStr.length(); j++)
					{
						if (str[j] != fromStr[j - i])
						{
							startFound = false;
							break;
						}
					}

					if (startFound)
					{
						from = i;
					}

					auto endFound = true;
					for (auto j = i; j < i + toStr.length(); j++)
					{
						if (str[j] != toStr[j - i])
						{
							endFound = false;
							break;
						}
					}

					if (endFound && from > -1)
					{
						to = i;
						break;
					}
				}

				str.erase(str.begin() + from, str.begin() + to+1);

				// All passes has been replaced.
				break;
			}
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
	
	void parse_shader(int source_length, std::string section)
	{
		auto has_input = false;
		auto has_output = false;
		std::string input = {};
		std::string output = {};
		std::vector<std::string> buffers = {};

		std::string section_parsed = section;

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

		// read shader sections
		for (auto i = 0; i < source.length(); i++)
		{
			auto line = compiler_utils::getLine(source, i);

			// read sections
			if (compiler_utils::startsWith(line, "VertexShader"))
			{
				auto section = compiler_utils::extractSection(source, i);

				parse_shader(int(section.length()), section);

				i += static_cast<int>(section.length());
				continue;
			}
			if (compiler_utils::startsWith(line, "PixelShader"))
			{
				auto section = compiler_utils::extractSection(source, i);

				parse_shader(int(section.length()), section);

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
