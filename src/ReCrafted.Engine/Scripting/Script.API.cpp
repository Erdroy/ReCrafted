// ReCrafted (c) 2016-2019 Always Too Late

#include "Script.h"

namespace Internal
{
    MonoObject* GetActor(Script* script)
    {
        DEBUG_ASSERT(script);
        return script->GetActor()->GetManagedPtr();
    }

    bool GetEnabled(Script* script)
    {
        DEBUG_ASSERT(script);
        return script->GetEnabled();
    }

    void SetEnabled(Script* script, const bool enabled)
    {
        DEBUG_ASSERT(script);
        script->SetEnabled(enabled);
    }

    API_DEFINE_OBJECT_CREATOR("ReCrafted.API.Common", "Script", Script)
}

void Script::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Common.Script", &Internal::CreateScript);

    API_FILE("Common/Script.Gen.cs");
    {
        API_USING("ReCrafted.API.Common.Components");
        API_USING("ReCrafted.API.Common.Actors");

        API_COMMENT("Script class");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "Script", "Object", PARTIAL);
        {
            API_COMMENT("The actor that contains this script.");
            API_PROPERTY(PUBLIC, REGULAR, "ActorBase", "Actor", GET);
            {
                API_BIND("ReCrafted.API.Common.Script::Internal_Actor_Get", &Internal::GetActor);
            }
            API_PROPERTY_END();

            API_COMMENT("The enable state of this script. True when enabled.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "Enabled", GETSET);
            {
                API_BIND("ReCrafted.API.Common.Script::Internal_Enabled_Get", &Internal::GetEnabled);
                API_BIND("ReCrafted.API.Common.Script::Internal_Enabled_Set", &Internal::SetEnabled);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
