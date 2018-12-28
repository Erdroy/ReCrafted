// ReCrafted (c) 2016-2018 Always Too Late

#include "WebUIView.h"
#include "Scripting/Mono.h"
#include "Scripting/Bindings.h"
#include "WebUI/WebUI.h"

namespace Internal
{
    void navigate(WebUIView* view, MonoString* string)
    {
        if (!view)
            return;

        // convert monostring to ansi string
        MONO_ANSI_ERR();
        auto str = MONO_ANSI(string);

        // navigate
        view->Navigate(str);

        // free ansi string
        MONO_ANSI_FREE(str);
    }

    void execute(WebUIView* view, MonoString* string)
    {
        if (!view)
            return;

        // convert monostring to ansi string
        MONO_ANSI_ERR();
        auto str = MONO_ANSI(string);

        // execute js
        view->Execute(str);

        // free ansi string
        MONO_ANSI_FREE(str);
    }

    MonoObject* createView()
    {
        return WebUI::GetInstance()->CreateView()->GetManagedPtr();
    }
}

void WebUIView::InitRuntime()
{
    // TODO: open/execute/bind methods

    API_FILE("WebUI/WebUIView.Gen.cs");
    {
        API_COMMENT("WebUIView class.");
        API_CLASS(PUBLIC, REGULAR, "ReCrafted.API.WebUI", "WebUIView", "Object", PARTIAL, NOCONSTRUCTOR);
        {
            API_COMMENT("Navigates current view to given URL.");
            API_METHOD(PUBLIC, REGULAR, "Navigate", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Navigate", &Internal::navigate);

                API_PARAM("string", "url");
            }
            API_METHOD_END();

            API_COMMENT("Executes given JavaScript code.");
            API_METHOD(PUBLIC, REGULAR, "ExecuteJS", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_ExecuteJS", &Internal::execute);

                API_PARAM("string", "javaScript");
            }
            API_METHOD_END();

            API_COMMENT("Creates new WebUIView");
            API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Create", &Internal::createView);

                API_RETURN("WebUIView");
            }
            API_METHOD_END();
        }
        API_CLASS_END()
    }
    API_FILE_END()
}
