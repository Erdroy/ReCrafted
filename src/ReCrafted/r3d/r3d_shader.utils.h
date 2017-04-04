// r3d © 2016-2017 Damian 'Erdroy' Korczowski

#ifndef R3D_SHADER_UTILS_H
#define R3D_SHADER_UTILS_H

// includes
#include <vector>
#include <string>

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

			str.erase(str.begin() + from, str.begin() + to + 1);

			// All passes has been replaced.
			break;
		}
	}

	static std::vector<std::string> splitString(std::string str, char delimeter)
	{
		std::vector<std::string> array = {};

		std::string word;
		for (auto i = 0; i < str.length(); i++)
		{
			auto ch = str[i];

			if (ch == delimeter)
			{
				array.push_back(word);
				word.clear();
			}
			else
			{
				word += ch;
			}
		}

		if (word.length() > 0)
			array.push_back(word);

		return array;
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

	static std::string readFromTo(std::string& source, char startCharacter, const char endCharacter, bool newlineExit)
	{
		auto start_pos = -1;

		// search for start
		for (auto i = 0u; i < source.size(); i++)
		{
			if (newlineExit && source[i] == '\n')
				return "";

			if (source[i] == startCharacter)
			{
				start_pos = i + 1;
				break;
			}
		}

		for (auto i = unsigned int(start_pos); i < source.size(); i++)
		{
			if (newlineExit && source[i] == '\n')
				return "";

			if (source[i] == endCharacter)
			{
				return source.substr(start_pos, i - start_pos);
			}
		}
		return "";
	}
};

#endif // R3D_SHADER_UTILS_H
