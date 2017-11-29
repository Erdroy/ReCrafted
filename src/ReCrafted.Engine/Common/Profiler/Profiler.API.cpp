// ReCrafted © 2016-2017 Always Too Late

#include "Profiler.h"
#include "Scripting/Mono.h"
#include "Common/ReCraftedAPI.h"

namespace Internal
{
	void beginProfile(MonoString* name)
	{
		var text = Text::constant(MONO_TEXT(name));
		Profiler::beginProfile(text);
	}
}

void Profiler::initRuntime()
{
	API_FILE("Common/Profiler.Gen.cs");
	{
		API_COMMENT("Profiler class.");
		API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "Profiler");
		{
			API_COMMENT("Begins profiling.");
			API_METHOD(PUBLIC, STATIC, "BeginProfile", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.Profiler::BeginProfile", &Internal::beginProfile);
				
				API_COMMENT("The profile name");
				API_PARAM("string", "name");

			}
			API_METHOD_END();

			API_COMMENT("Ends profiling.");
			API_METHOD(PUBLIC, STATIC, "EndProfile", EXTERN);
			{
				API_BIND("ReCrafted.API.Common.Profiler::EndProfile", &Profiler::endProfile);

			}
			API_METHOD_END();
		}
		API_CLASS_END();
	}
	API_FILE_END();
}