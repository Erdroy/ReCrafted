// ReCrafted (c) 2016-2018 Always Too Late

#include "Profiler.h"
#include "Scripting/Mono.h"
#include "Common/ReCraftedAPI.h"

namespace Internal
{
    void BeginProfile(MonoString* name, float med, float max)
    {
        //var text = MONO_TEXT(name);
        //Profiler::BeginProfile(text, med, max);
    }

    void EndProfile()
    {
        //Profiler::EndProfile();
    }
}

void Profiler::InitRuntime()
{
    API_FILE("Common/Profiler.Gen.cs");
    {
        API_COMMENT("Profiler class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Common", "Profiler");
        {
            API_COMMENT("Begins profiling.");
            API_METHOD(PUBLIC, STATIC, "BeginProfile", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Profiler::BeginProfile", &Internal::BeginProfile);

                API_COMMENT("The profile name");
                API_PARAM("string", "name");

                API_COMMENT("The warning limit");
                API_PARAM("float", "warm = -1.0f");

                API_COMMENT("The max limit");
                API_PARAM("float", "max = -1.0f");
            }
            API_METHOD_END();

            API_COMMENT("Ends profiling.");
            API_METHOD(PUBLIC, STATIC, "EndProfile", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Profiler::EndProfile", &Internal::EndProfile);
            }
            API_METHOD_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
