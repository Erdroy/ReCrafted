// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Input.h"
#include "Scripting/ScriptingEngine.h"
#include "Common/ReCraftedAPI.h"

namespace Internal
{
	bool IsKeyDown(int key)
	{
		return Input::isKeyDown(static_cast<Keys>(key));
	}

	bool IsKeyUp(int key)
	{
		return Input::isKeyUp(static_cast<Keys>(key));
	}

	bool IsKey(int key)
	{
		return Input::isKey(static_cast<Keys>(key));
	}

	void SetCursorPosition(Vector2 position)
	{
		Input::setCursorPos(static_cast<int>(position.x), static_cast<int>(position.y));
	}
}

void Input::initRuntime()
{
	API_FILE("Common/Input.Gen.cs");
	{
		API_USING("ReCrafted.API.Core");

		API_COMMENT("Input class.");
		API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "Input");
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

			API_COMMENT("Checks if the key is not pressed.");
			API_METHOD(PUBLIC, STATIC, "IsKeyUp", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.Input::IsKeyUp", &Internal::IsKeyUp);
				API_COMMENT("The key");
				API_PARAM("Keys", "key");

				API_RETURN("bool");
			}
			API_METHOD_END();

			API_COMMENT("Checks if the key held.");
			API_METHOD(PUBLIC, STATIC, "IsKey", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.Input::IsKey", &Internal::IsKey);
				API_COMMENT("The key");
				API_PARAM("Keys", "key");

				API_RETURN("bool");
			}
			API_METHOD_END();

			API_COMMENT("Changes cursor's current positon.");
			API_METHOD(PUBLIC, STATIC, "SetCursorPosition", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.Input::setCursorPos", &Internal::SetCursorPosition);
				API_COMMENT("Cursor's position");
				API_PARAM("Vector2", "position");
			}
			API_METHOD_END();

			API_COMMENT("Returns cursor's current position.");
			API_PROPERTY(PUBLIC, STATIC, "Vector2", "GetCursorPosition", GET);
			{
				API_BIND("ReCrafted.API.Common.Input::getCursorPos", &Input::getCursorPos);
			}
			API_PROPERTY_END();

			API_COMMENT("Returns cursor's position change since last frame.");
			API_PROPERTY(PUBLIC, STATIC, "Vector2", "GetCursorDelta", GET);
			{
				API_BIND("ReCrafted.API.Common.Input::getCursorDelta", &Input::getCursorDelta);
			}
			API_PROPERTY_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}