// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

#include "MonoJSCore.h"
#include "Scripting/Mono.h"

#include <JavaScriptCore/JavaScript.h>
#include "Common/Logger.h"

bool MonoJSCore::m_initialized;

JSValueRef JSNativeCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject,
    size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception) {
    
    auto callback = (MonoObject*)JSObjectGetPrivate(function);
    
    if (!callback)
        return JSValueMakeNull(ctx);

    auto rV = mono_runtime_delegate_invoke(callback, (void**)& thisObject, nullptr);

    auto returnValue = (JSValueRef)mono_object_unbox(rV);

    auto isNull = JSValueIsNull(ctx, returnValue);

    return returnValue;
}

JSClassRef JSGetNativeCallbackClass() {
    static JSClassRef instance = nullptr;
    if (!instance) {
        JSClassDefinition def;
        memset(&def, 0, sizeof(def));
        def.className = "JSNativeCallbackClass";
        def.attributes = kJSClassAttributeNone;
        def.callAsFunction = JSNativeCallback;
        instance = JSClassCreate(&def);
    }
    return instance;
}

void MonoJSCore::Initialize()
{
    if (m_initialized)
        return;
    m_initialized = true;

    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSGetNativeCallbackClass", &JSGetNativeCallbackClass);

    // JSBase.h
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSEvaluateScript", &JSEvaluateScript);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSCheckScriptSyntax", &JSCheckScriptSyntax);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSGarbageCollect", &JSGarbageCollect);

    // JSContextRef.h

    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSContextGroupCreate", &JSContextGroupCreate);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSContextGroupRetain", &JSContextGroupRetain);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSContextGroupRelease", &JSContextGroupRelease);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSGlobalContextCreate", &JSGlobalContextCreate);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSGlobalContextCreateInGroup", &JSGlobalContextCreateInGroup);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSGlobalContextRetain", &JSGlobalContextRetain);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSGlobalContextRelease", &JSGlobalContextRelease);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSContextGetGlobalObject", &JSContextGetGlobalObject);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSContextGetGroup", &JSContextGetGroup);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSContextGetGlobalContext", &JSContextGetGlobalContext);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSGlobalContextCopyName", &JSGlobalContextCopyName);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSGlobalContextSetName", &JSGlobalContextSetName);

    // JSStringRef.h

    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSStringCreateWithUTF8CString", &JSStringCreateWithUTF8CString);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSStringRetain", &JSStringRetain);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSStringRelease", &JSStringRelease);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSStringGetLength", &JSStringGetLength);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSStringGetUTF8CString", &JSStringGetUTF8CString);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSStringIsEqual", &JSStringIsEqual);

    // JSObjectRef.h

    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSClassRetain", &JSClassRetain);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSClassRelease", &JSClassRelease);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectMake", &JSObjectMake);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectMakeCallback", &JSObjectMake);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectHasProperty", &JSObjectHasProperty);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectGetProperty", &JSObjectGetProperty);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectSetProperty", &JSObjectSetProperty);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectDeleteProperty", &JSObjectDeleteProperty);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectGetPrivate", &JSObjectGetPrivate);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectSetPrivate", &JSObjectSetPrivate);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectIsFunction", &JSObjectIsFunction);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSObjectCallAsFunction", &JSObjectCallAsFunction);

    // JSValueRef.h

    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueGetType", &JSValueGetType);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueIsUndefined", &JSValueIsUndefined);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueIsBoolean", &JSValueIsBoolean);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueIsNumber", &JSValueIsNumber);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueIsString", &JSValueIsString);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueIsObject", &JSValueIsObject);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueIsArray", &JSValueIsArray);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueIsEqual", &JSValueIsEqual);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueIsStrictEqual", &JSValueIsStrictEqual);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueMakeUndefined", &JSValueMakeUndefined);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueMakeNull", &JSValueMakeNull);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueMakeBoolean", &JSValueMakeBoolean);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueMakeNumber", &JSValueMakeNumber);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueMakeString", &JSValueMakeString);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueToBoolean", &JSValueToBoolean);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueToNumber", &JSValueToNumber);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueToStringCopy", &JSValueToStringCopy);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueToObject", &JSValueToObject);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueProtect", &JSValueProtect);
    API_BIND("ReCrafted.API.WebUI.JavaScript.JSCore::JSValueUnprotect", &JSValueUnprotect);

    // Done.
    Logger::Log("MonoJSCore initialized");
}
