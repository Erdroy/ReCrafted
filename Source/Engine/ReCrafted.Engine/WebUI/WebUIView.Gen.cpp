// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

#include "WebUIView.h"
#include "Scripting/Mono.h"
#include "Scripting/ScriptingManager.h"
#include "Scripting/Assembly.h"

class APIProxy
{
public:
    
    static void WebUIView_Resize1(WebUIView* instance, uint width, uint height) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->Resize(width, height);
    }
    
    static void WebUIView_Navigate2(WebUIView* instance, MonoString* p_url) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_url);
        const auto url = MONO_STRING_TO_CSTR(p_url);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->Navigate(url);
        MONO_FREE(url);
    }
    
    static void WebUIView_Execute3(WebUIView* instance, MonoString* p_javaScriptSource) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_javaScriptSource);
        const auto javaScriptSource = MONO_STRING_TO_CSTR(p_javaScriptSource);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->Execute(javaScriptSource);
        MONO_FREE(javaScriptSource);
    }
    
    static void WebUIView_AddOnBeginLoading4(WebUIView* instance, MonoObject* p_action) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_action);
        const auto action = MONO_DELEGATE_TO_ACTION_1(p_action, void, void, void, _t0);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->AddOnBeginLoading(action);
        MONO_FREE_STUB(action);
    }
    
    static void WebUIView_RemoveOnBeginLoading5(WebUIView* instance, MonoObject* p_action) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_action);
        const auto action = MONO_DELEGATE_TO_ACTION_1(p_action, void, void, void, _t0);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->RemoveOnBeginLoading(action);
        MONO_FREE_STUB(action);
    }
    
    static void WebUIView_AddOnFinishLoading6(WebUIView* instance, MonoObject* p_action) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_action);
        const auto action = MONO_DELEGATE_TO_ACTION_1(p_action, void, void, void, _t0);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->AddOnFinishLoading(action);
        MONO_FREE_STUB(action);
    }
    
    static void WebUIView_RemoveOnFinishLoading7(WebUIView* instance, MonoObject* p_action) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_action);
        const auto action = MONO_DELEGATE_TO_ACTION_1(p_action, void, void, void, _t0);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->RemoveOnFinishLoading(action);
        MONO_FREE_STUB(action);
    }
    
    static void WebUIView_AddOnDOMReady8(WebUIView* instance, MonoObject* p_action) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_action);
        const auto action = MONO_DELEGATE_TO_ACTION_1(p_action, void, void, void, _t0);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->AddOnDOMReady(action);
        MONO_FREE_STUB(action);
    }
    
    static void WebUIView_RemoveOnDOMReady9(WebUIView* instance, MonoObject* p_action) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_action);
        const auto action = MONO_DELEGATE_TO_ACTION_1(p_action, void, void, void, _t0);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->RemoveOnDOMReady(action);
        MONO_FREE_STUB(action);
    }
    
    static void WebUIView_Get_Active1(WebUIView* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "WebUIView");
        const auto _returnValue = instance->GetActive();
        *data = _returnValue;
    }

    static void WebUIView_Set_Active1(WebUIView* instance, bool* data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->SetActive(*data);
    }
    
    static void WebUIView_Get_Name2(WebUIView* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "WebUIView");
        const auto _returnValue = instance->GetName();
        *p_data = MONO_STRING_FROM_STR(_returnValue);
    }

    static void WebUIView_Set_Name2(WebUIView* instance, MonoString** p_data) 
    {
        MAIN_THREAD_ONLY();
        ASSERT(p_data);
        const auto data = MONO_STRING_TO_STR(*p_data);
        MONO_CHECK_OBJECT(instance, "WebUIView");
        instance->SetName(data);
        MONO_FREE_STUB(data);
    }
    
    static void WebUIView_Get_Context3(WebUIView* instance, void** data) 
    {
        MAIN_THREAD_ONLY();
        MONO_CHECK_OBJECT(instance, "WebUIView");
        const auto _returnValue = instance->Context();
        *data = _returnValue;
    }
    
    static Object* WebUIView_CreateObject(bool createManagedInstance)
    {
        _ASSERT_(createManagedInstance, "Class 'WebUIView' is not marked as generic, and thus cannot get only unmanaged-instance created!");
        return Object::New<WebUIView>();
    }
};

void WebUIView::InitRuntime() 
{
    MONO_REGISTER_OBJECT(&APIProxy::WebUIView_CreateObject);
    
    MONO_REGISTER_OBJECT_TYPE(WebUIView);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalResize", &APIProxy::WebUIView_Resize1);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalNavigate", &APIProxy::WebUIView_Navigate2);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalExecute", &APIProxy::WebUIView_Execute3);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalAddOnBeginLoading", &APIProxy::WebUIView_AddOnBeginLoading4);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalRemoveOnBeginLoading", &APIProxy::WebUIView_RemoveOnBeginLoading5);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalAddOnFinishLoading", &APIProxy::WebUIView_AddOnFinishLoading6);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalRemoveOnFinishLoading", &APIProxy::WebUIView_RemoveOnFinishLoading7);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalAddOnDOMReady", &APIProxy::WebUIView_AddOnDOMReady8);
    API_BIND("ReCrafted.API.WebUI.WebUIView::InternalRemoveOnDOMReady", &APIProxy::WebUIView_RemoveOnDOMReady9);
    API_BIND("ReCrafted.API.WebUI.WebUIView::Get_InternalActive", &APIProxy::WebUIView_Get_Active1);
    API_BIND("ReCrafted.API.WebUI.WebUIView::Set_InternalActive", &APIProxy::WebUIView_Set_Active1);
    API_BIND("ReCrafted.API.WebUI.WebUIView::Get_InternalName", &APIProxy::WebUIView_Get_Name2);
    API_BIND("ReCrafted.API.WebUI.WebUIView::Set_InternalName", &APIProxy::WebUIView_Set_Name2);
    API_BIND("ReCrafted.API.WebUI.WebUIView::Get_InternalContext", &APIProxy::WebUIView_Get_Context3);
}

const char* WebUIView::Fullname() 
{
    return "ReCrafted.API.WebUI.WebUIView";
}

const char* WebUIView::Name() 
{
    return "WebUIView";
}

const char* WebUIView::Namespace() 
{
    return "ReCrafted.API.WebUI";
}