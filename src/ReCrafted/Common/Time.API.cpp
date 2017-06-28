// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

#include "Time.h"
#include "../Common/ReCraftedAPI.h"
#include "../Scripting/ScriptingEngine.h"

void Time::initRuntime()
{
	API_FILE("Common/Time.Gen.cs")
	{
		API_COMMENT("Time class.");
		API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "Time");
		{
			API_COMMENT("Time between current and last frame.");
			API_PROPERTY(PUBLIC, STATIC, "double", "DeltaTime", GET);
			{
				API_BIND("ReCrafted.API.Common.Time::Internal_DeltaTime_Get", &Time::deltaTime);
			}

			API_COMMENT("Current time.");
			API_PROPERTY(PUBLIC, STATIC, "double", "CurrentTime", GET);
			{
				API_BIND("ReCrafted.API.Common.Time::Internal_CurrentTime_Get", &Time::time);
			}

			API_COMMENT("Current frame count");
			API_PROPERTY(PUBLIC, STATIC, "int", "Frames", GET);
			{
				API_BIND("ReCrafted.API.Common.Time::Internal_Frames_Get", &Time::frames);
			}
		}
		API_CLASS_END();
	}
	API_FILE_END();
}