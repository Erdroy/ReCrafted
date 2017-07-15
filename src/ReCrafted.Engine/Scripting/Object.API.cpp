// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Object.h"
#include "Scripting/Mono.h"

void Object::initRuntime()
{
	API_FILE("Object.Gen.cs");
	{
		API_USING("System");

		API_COMMENT("Base class for all mono object instances.");
		API_CLASS(PUBLIC, ABSTRACT, "ReCrafted.API", "Object", PARTIAL);
		{
			API_METHOD(INTERNAL, STATIC, "InternalDestroy", EXTERN);
			{
				//API_BIND("ReCrafted.API.Core.Game::InternalDestroy", &ScriptingEngine::destroy);

				API_PARAM("IntPtr", "nativePtr");
			}
			API_METHOD(INTERNAL, STATIC, "InternalObjectFinalized", EXTERN);
			{
				//API_BIND("ReCrafted.API.Core.Game::InternalObjectFinalized", &ScriptingEngine::onFinalized);

				API_PARAM("IntPtr", "nativePtr");
			}
		}
		API_CLASS_END();
	}
	API_FILE_END();
}
