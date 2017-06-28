// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#pragma once

#define API_BUILD

// mono extensions
#define API_DEF friend class ScriptingEngine; protected: static void initRuntime(); private:

#define MONO_TEXT(x) mono_string_chars(string)

#define API_BIND(name, method) mono_add_internal_call(name, method)

// API
#define API_FILE(file_name)
#define API_FILE_END()

// common
#define API_COMMENT(comment)

#define PUBLIC
#define PROTECTED
#define INTERNAL
#define PRIVATE

#define REGULAR
#define OVERRIDE
#define STATIC
#define VIRTUAL
#define SEALED
#define ABSTRACT

// flags
#define PARTIAL
#define EXTERN

#define REF
#define OUT

// usings
#define API_USING(using_name)

// class
#define API_CLASS(access, type, namespace, class_name, ...) //, base_name, PARTIAL
#define API_CLASS_END()

// struct
// TODO: struct

// enum
// TODO: enums

// method
#define API_METHOD(access, type, method_name, ...) //, EXTERN
#define API_METHOD_END()
#define API_PARAM(type, name, ...) //, REF/OUT 
#define API_RETURN(type)
#define API_CODE(code)

// property
// TODO: properties

/*
 *API_FILE("Core/ILogger.Gen.cs");
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
 */