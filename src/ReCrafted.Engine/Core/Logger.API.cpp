// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Logger.h"
#include "Scripting/Mono.h"
#include "Common/ReCraftedAPI.h"

namespace Internal
{
	void Write(MonoString* string, int level)
	{
		auto text = MONO_TEXT(string);
		auto logLevel = static_cast<LogLevel::Enum>(level);

		Logger::write(text, logLevel);
	}
}

void Logger::initRuntime()
{
	API_FILE("Core/Logger.Gen.cs");
	{
		API_COMMENT("Logger class. Writes debug messages to output file.");
		API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Core", "Logger", PARTIAL);
		{
			API_COMMENT("Writes message to the output file.");
			API_METHOD(INTERNAL, STATIC, "Internal_Write", EXTERN);
			{
				API_BIND("ReCrafted.API.Core.Logger::Internal_Write", &Internal::Write);
				API_COMMENT("The message");
				API_PARAM("string", "message");
				API_PARAM("LogLevel", "level");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}