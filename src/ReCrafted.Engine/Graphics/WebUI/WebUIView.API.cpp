// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"
#include "Scripting/Mono.h"
#include "Scripting/Bindings.h"
#include "Common/Text.h"

namespace Internal
{
    void navigate(WebUIView* view, MonoString* string)
    {
        cvar url = Text::constant(MONO_TEXT(string));

        if (view)
            view->navigate(url);
    }
}

void WebUIView::initRuntime()
{
    // TODO: open/execute/bind methods

    API_FILE("Graphics/WebUIView.Gen.cs");
    {
        API_COMMENT("WebUIView class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.Graphics", "WebUIView", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("Navigates current view to given URL.");
            API_METHOD(PUBLIC, REGULAR, "Navigate", EXTERN);
            {
                API_BIND("ReCrafted.API.Graphics.WebUIView::Internal_Navigate", &Internal::navigate);

                API_PARAM("string", "url");
            }
            API_METHOD_END();
        }
        API_CLASS_END()
    }
    API_FILE_END()
}
