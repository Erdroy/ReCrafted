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
	API_FILE("Core/Logger.cs");
	{
		API_USING("System");
		API_USING("System.Linq");
		API_USING("System.Collections.Generic");

		API_COMMENT("Logger class");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API", "Logger");
		{
			API_COMMENT("Writes message to the output.");
			API_METHOD(PUBLIC, STATIC, "Write");
			{
				API_BIND("ReCrafted.Core.Logger::Write", &Internal::Write);

				API_COMMENT("The message");
				API_PARAM("string", "message");
			}
			API_METHOD_END();

			API_COMMENT("Test method.");
			API_METHOD(PUBLIC, REGULAR, "Test");
			{
				API_COMMENT("The message");
				API_PARAM("string", "message");
				API_PARAM_REF("int", "Test");
				API_PARAM_OUT("string", "outstr");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}