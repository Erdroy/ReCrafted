// ReCrafted © 2016-2017 Always Too Late

#include "Script.h"

namespace Internal
{
    void createScript(MonoObject* instance)
    {
        Ptr<Script> script(new Script);
        Object::initializeInstance(script, instance);
    }
}

void Script::initRuntime()
{
    API_FILE("Common/Script.Gen.cs")
    {
        API_COMMENT("Script base class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "Script", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("Registers given script to new native script object.");
            API_METHOD(INTERNAL, STATIC, "Register", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Script::Register", &Internal::createScript);

                API_PARAM("Script", "instance");
            }
            API_METHOD_END();

        }
        API_CLASS_END();
    }
    API_FILE_END();
}
