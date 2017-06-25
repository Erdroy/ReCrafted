// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Logger.h"
#include "../Scripting/ScriptingEngine.h"
#include "../Common/ReCraftedAPI.h"

namespace Internal
{
	void Write(MonoString* string)
	{
	}
}

void Logger::initRuntime()
{
	API_FILE("Core/ILogger.Gen.cs");
	{
		API_USING("System");
		API_USING("System.Linq");
		API_USING("System.Collections.Generic");

		API_COMMENT("Logger base class");
		API_CLASS(PUBLIC, ABSTRACT, "ReCrafted.API.Core", "ILogger");
		{
			API_COMMENT("Test method.");
			API_METHOD(PUBLIC, ABSTRACT, "Test");
			{
				API_COMMENT("The message");
				API_PARAM("string", "message");
				API_PARAM("int", "test", REF);
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();

	API_FILE("Core/Logger.Gen.cs");
	{
		API_USING("System");
		API_USING("System.Linq");
		API_USING("System.Collections.Generic");

		API_COMMENT("Logger class");
		API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Core", "Logger", "ILogger", PARTIAL);
		{
			API_COMMENT("Writes message to the output.");
			API_METHOD(PUBLIC, STATIC, "Write", EXTERN);
			{
				API_BIND("ReCrafted.API.Core.Logger::Write", &Internal::Write);

				API_COMMENT("The message");
				API_PARAM("string", "message");
			}
			API_METHOD_END();

			API_COMMENT("Test method.");
			API_METHOD(PUBLIC, OVERRIDE, "Test");
			{
				API_COMMENT("The message");
				API_PARAM("string", "message");
				API_PARAM("int", "test", REF);
			}
			API_METHOD_END();

			API_METHOD(PRIVATE, REGULAR, "Test2");
			{
				API_COMMENT("The message");
				API_PARAM("string", "message");
				API_PARAM("int", "test", REF);
				API_PARAM("string", "outstr", OUT);

				API_CODE("outstr = message + test.ToString();");
				API_CODE("return outstr;");

				API_RETURN("string");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}