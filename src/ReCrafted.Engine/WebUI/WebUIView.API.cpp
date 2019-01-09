// ReCrafted (c) 2016-2019 Always Too Late

#include "WebUIView.h"
#include "Scripting/Mono.h"
#include "Scripting/Bindings.h"
#include "WebUI/WebUI.h"

namespace Internal
{
    void WebUIView_Navigate(WebUIView* view, MonoString* string)
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

    void WebUIView_Execute(WebUIView* view, MonoString* string)
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

    void WebUIView_Bind(WebUIView* view, MonoString* string, MonoObject* callback)
    {
        if (!view)
            return;

        MONO_ANSI_ERR();

        // Get bind name
        cvar bindName = MONO_ANSI(string);

        // Bind JS callback
        view->Bind(bindName, Delegate::FromMonoObject(callback));

        // Cleanup
        MONO_ANSI_FREE(bindName);
    }

    void* WebUIView_Call(WebUIView* view, MonoString* string, MonoType* returnType, MonoArray* parameters)
    {
        if (!view)
            return nullptr;

        MONO_ANSI_ERR();

        // Get function name
        cvar functionName = MONO_ANSI(string);

        // Call JS function
        cvar returnData = view->Call(functionName, returnType, parameters);

        // Cleanup
        MONO_ANSI_FREE(functionName);

        return returnData;
    }

    bool WebUIView_GetActive(WebUIView* view)
    {
        ASSERT(view);
        return view->GetActive();
    }
    void WebUIView_SetActive(WebUIView* view, const bool active)
    {
        ASSERT(view);
        view->SetActive(active);
    }

    MonoObject* WebUIView_Create()
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
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Navigate", &Internal::WebUIView_Navigate);

                API_PARAM("string", "url");
            }
            API_METHOD_END();

            API_COMMENT("Executes given JavaScript code.");
            API_METHOD(PUBLIC, REGULAR, "ExecuteJS", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_ExecuteJS", &Internal::WebUIView_Execute);

                API_PARAM("string", "javaScript");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Action", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<T1>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<T1>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Action<T1>", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<T1, T2>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<T1, T2>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Action<T1, T2>", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<T1, T2, T3>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<T1, T2, T3>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Action<T1, T2, T3>", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<T1, T2, T3, T4>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<T1, T2, T3, T4>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Action<T1, T2, T3, T4>", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<TReturn>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<TReturn>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Func<TReturn>", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<TReturn, T1>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<TReturn, T1>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Func<TReturn, T1>", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<TReturn, T1, T2>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<TReturn, T1, T2>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Func<TReturn, T1, T2>", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<TReturn, T1, T2, T3>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<TReturn, T1, T2, T3>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Func<TReturn, T1, T2, T3>", "function");
            }
            API_METHOD_END();

            API_COMMENT("Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!");
            API_METHOD(PUBLIC, REGULAR, "Bind<TReturn, T1, T2, T3, T4>", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Bind<TReturn, T1, T2, T3, T4>", &Internal::WebUIView_Bind);

                API_PARAM("string", "name");
                API_PARAM("Func<TReturn, T1, T2, T3, T4>", "function");
            }
            API_METHOD_END();

            API_METHOD(PRIVATE, REGULAR, "Call", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Call", &Internal::WebUIView_Call);

                API_PARAM("string", "name");
                API_PARAM("IntPtr", "returnType");
                API_PARAM("params object[]", "parameters");

                API_RETURN("object");
            }
            API_METHOD_END();

            API_COMMENT("Creates new WebUIView");
            API_METHOD(PUBLIC, STATIC, "Create", EXTERN);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Create", &Internal::WebUIView_Create);

                API_RETURN("WebUIView");
            }
            API_METHOD_END();

            API_COMMENT("Gets or sets the active state.");
            API_PROPERTY(PUBLIC, REGULAR, "bool", "Active", GETSET);
            {
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Active_Get", &Internal::WebUIView_GetActive);
                API_BIND("ReCrafted.API.WebUI.WebUIView::Internal_Active_Set", &Internal::WebUIView_SetActive);
            }
            API_PROPERTY_END();
        }
        API_CLASS_END()
    }
    API_FILE_END()
}
