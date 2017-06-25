// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Logger.h"
#include "../Scripting/ScriptingEngine.h"
#include "../Common/ReCraftedAPI.h"

namespace Internal
{
	void Write(MonoString* string)
	{
		Logger::write("Test!");
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
			API_METHOD(PUBLIC, STATIC, "Write", EXTERN);
			{
				API_BIND("ReCrafted.API.Core.Logger::Write", &Internal::Write);
				API_COMMENT("The message");
				API_PARAM("string", "message");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}