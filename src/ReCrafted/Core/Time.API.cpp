// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Time.h"
#include "../Common/ReCraftedAPI.h"
#include "../Scripting/ScriptingEngine.h"

namespace Internal
{
	double DeltaTime()
	{
		return Time::deltaTime();
	}

	float CurrentTime()
	{
		return Time::time();
	}

	int Frames()
	{
		return Time::frames();
	}
}

void Time::initRuntime()
{
	API_FILE("Common/Time.Gen.cs")
	{
		API_COMMENT("Time class.");
		API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "Time");
		{
			API_COMMENT("Returns time between current and last frame.");
			API_METHOD(PUBLIC, STATIC, "DeltaTime", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.Time::DeltaTime", &Internal::DeltaTime);

				API_RETURN("double");
			}
			API_METHOD_END();

			API_COMMENT("Returns current time");
			API_METHOD(PUBLIC, STATIC, "CurrentTime", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.Time::CurrentTime", &Internal::CurrentTime);

				API_RETURN("float");
			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}