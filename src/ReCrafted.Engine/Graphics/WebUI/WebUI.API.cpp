// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUI.h"
#include "Scripting/Object.h"
#include "WebUIView.h"

namespace Internal
{
    MonoObject* createView()
    {
        return WebUI::getInstance()->createView()->getManagedPtr();
    }
}

void WebUI::initRuntime()
{
    API_FILE("Graphics/WebUI.Gen.cs");
    {
        API_COMMENT("WebUI class.");
        API_CLASS(PUBLIC, STATIC, "ReCrafted.API.Graphics", "WebUI");
        {
            API_COMMENT("Creates new WebUIView");
            API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.WebUI::Create", &Internal::createView);

                API_RETURN("WebUIView");
            }
            API_METHOD_END();
        }
        API_CLASS_END()
    }
    API_FILE_END()
}
