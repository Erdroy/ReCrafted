#pragma once

#ifndef JSHELPERS_H
#define JSHELPERS_H

#include <JavaScriptCore/JavaScript.h>
#include <Ultralight/String.h>

/**
* This is a set of common JavaScriptCore Helper functions
* to simplify sample code.
*/

// Call JS Function object with a single boolean argument
inline void CallJS(JSContextRef ctx, JSObjectRef obj, bool val) {
    JSValueRef args[] = { JSValueMakeBoolean(ctx, val) };
    JSObjectCallAsFunction(ctx, obj, 0, 1, args, 0);
}

// Call JS Function object with a single double argument
inline void CallJS(JSContextRef ctx, JSObjectRef obj, double val) {
    JSValueRef args[] = { JSValueMakeNumber(ctx, val) };
    JSObjectCallAsFunction(ctx, obj, 0, 1, args, 0);
}

// Call JS Function object with a single string argument
inline void CallJS(JSContextRef ctx, JSObjectRef obj, const ultralight::String& val) {
    JSStringRef str = JSStringCreateWithCharacters(val.utf16().data(), val.utf16().length());
    JSValueRef args[] = { JSValueMakeString(ctx, str) };
    JSObjectCallAsFunction(ctx, obj, 0, 1, args, 0);
}

// Call JS Function object with arbitrary number of arguments
inline void CallJS(JSContextRef ctx, JSObjectRef obj, size_t num_args, const JSValueRef* args) {
    JSObjectCallAsFunction(ctx, obj, 0, num_args, args, 0);
}

// Create JSString from null-terminated C-string
inline JSStringRef JSStr(const char* str) {
    return JSStringCreateWithUTF8CString(str);
}

// Create JSString from ultralight::String
inline JSStringRef JSStr(const ultralight::String& val) {
    return JSStringCreateWithCharacters(val.utf16().data(), val.utf16().length());
}

// Convert JSString to ultralight::String
inline ultralight::String FromJSStr(JSStringRef str) {
    return ultralight::String16(JSStringGetCharactersPtr(str), JSStringGetLength(str));
}

// Get JS Function object by name
inline JSObjectRef JSGetFunction(JSContextRef ctx, const char* str) {
    JSValueRef result = JSEvaluateScript(ctx, JSStr(str), 0, 0, 0, 0);
    if (!JSValueIsObject(ctx, result)) return nullptr;
    JSObjectRef obj = JSValueToObject(ctx, result, 0);
    if (!JSObjectIsFunction(ctx, obj)) return nullptr;
    return obj;
}

// Bind C callback to a certain named JavaScript function
inline void BindJSCallback(JSContextRef ctx, const char* str, JSObjectCallAsFunctionCallback cb) {
    JSObjectRef globalObject = JSContextGetGlobalObject(ctx);
    JSStringRef funcName = JSStr(str);
    JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, funcName, cb);
    JSValueRef exception = nullptr;
    JSObjectSetProperty(ctx, globalObject, funcName, func, 0, &exception);
}


#endif // JSHELPERS_H