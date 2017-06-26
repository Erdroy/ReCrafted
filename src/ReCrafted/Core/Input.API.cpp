// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Input.h"
#include "../Scripting/ScriptingEngine.h"
#include "../Common/ReCraftedAPI.h"

namespace Internal
{
	bool IsKeyDown(int key)
	{
		return Input::isKeyDown(static_cast<Keys>(key));
	}
}

void Input::initRuntime()
{
	API_FILE("Common/Input.Gen.cs");
	{
		API_USING("ReCrafted.API.Core");

		API_COMMENT("Input class.");
		API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "Input", PARTIAL);
		{
			API_COMMENT("Checks if the key is pressed down.");
			API_METHOD(PUBLIC, STATIC, "IsKeyDown", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.Input::IsKeyDown", &Internal::IsKeyDown);
				API_COMMENT("The key");
				API_PARAM("Keys", "key");

				API_RETURN("bool");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}