// ReCrafted (c) 2016-2018 Always Too Late

#include "Shader.h"
#include "Scripting/ScriptingAPI.h"

namespace Internal
{
    API_DEFINE_OBJECT_CREATOR("ReCrafted.API.Graphics", "Shader", Shader)
}

void Shader::InitRuntime()
{
    API_REGISTER_OBJECT("ReCrafted.API.Graphics.Shader", &Internal::CreateShader);

    API_FILE("Graphics/Shader.Gen.cs");
    {
        API_COMMENT("Shader class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "Shader", "Object", PARTIAL, NOCONSTRUCTOR);
        {
        }
        API_CLASS_END();
    }
    API_FILE_END();
}
