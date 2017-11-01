// ReCrafted © 2016-2017 Always Too Late

#include "Script.h"

namespace Internal
{
    MonoObject* createScript()
    {
        return Object::createInstance<Script>("ReCrafted.API.Common", "Script")->getManagedPtr();
    }
}

void Script::initRuntime()
{
    API_FILE("Common/Script.Gen.cs")
    {

        API_COMMENT("Script base class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Common", "Script", "Object", PARTIAL);
        {
            API_COMMENT("Creates new Script");
            API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
            {
                API_BIND("ReCrafted.API.Common.Script::Create", &Internal::createScript);

                API_RETURN("Script");
            }
            API_METHOD_END();

        }
        API_CLASS_END();
    }
    API_FILE_END();
}
