// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Logger.h"
#include "../Scripting/ScriptingEngine.h"

namespace Internal
{
	void Write(MonoString* string)
	{
	}
}

void Logger::initRuntime()
{
	API_BEGIN("Core/ILogger.cs");
	{
		API_COMMENT("ILogger class");
		API_CLASS("ReCrafted.Core", "ILogger", ABSTRACT);
		{
			API_COMMENT("Writes message to the output.");
			API_METHOD("Write", STATIC, ABSTRACT);
			{
				API_COMMENT("The message");
				API_PARAM("string", "message");
			}
			API_PUSH();
		}
		API_PUSH();
	}
	API_END();

	API_BEGIN("Core/Logger.cs");
	{
		API_COMMENT("Logger class");
		API_CLASS("ReCrafted.Core", "Logger", SEALED, INHERIT("ILogger"));
		{
			API_COMMENT("Writes message to the output.");
			API_METHOD("Write", STATIC, OVERRIDE);
			{
				API_BIND("ReCrafted.Core.Logger::Write", &Internal::Write);

				API_COMMENT("The message");
				API_PARAM("string", "message");
			}
			API_PUSH();
		}
		API_PUSH();
	}
	API_END();
}