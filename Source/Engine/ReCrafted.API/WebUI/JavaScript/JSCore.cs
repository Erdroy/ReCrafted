// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.WebUI.JavaScript
{
    internal static unsafe class JSCore
    {
        public delegate IntPtr JSNativeCallback(params IntPtr[] args);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSGetNativeCallbackClass();

        // JSBase.h

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSEvaluateScript(IntPtr context, IntPtr script, IntPtr thisObject, IntPtr sourceURL, int startingLineNumber, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSCheckSyntax(IntPtr context, IntPtr script, IntPtr sourceURL, int startingLineNumber, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void JSGarbageCollect(IntPtr context);

        // JSContextRef.h

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSContextGroupCreate();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSContextGroupRetain(IntPtr group);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void JSContextGroupRelease(IntPtr group);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSGlobalContextCreate(IntPtr globalObjectClass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSGlobalContextCreateInGroup(IntPtr group, IntPtr globalObjectClass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSGlobalContextRetain(IntPtr context);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void JSGlobalContextRelease(IntPtr context);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSContextGetGlobalObject(IntPtr context);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSContextGetGroup(IntPtr context);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSContextGetGlobalContext(IntPtr context);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSGlobalContextCopyName(IntPtr context);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void JSGlobalContextSetName(IntPtr context, IntPtr name);

        // JSStringRef.h

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSStringCreateWithUTF8CString(byte* str);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSStringRetain(IntPtr str);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void JSStringRelease(IntPtr str);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern ulong JSStringGetLength(IntPtr str);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern ulong JSStringGetUTF8CString(IntPtr str, byte* buffer, ulong bufferSize);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSStringIsEqual(IntPtr a, IntPtr b);

        // JSObjectRef.h

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSClassRetain(IntPtr jsClass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void JSClassRelease(IntPtr jsClass);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSObjectMake(IntPtr context, IntPtr jsClass, IntPtr data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSObjectMakeCallback(IntPtr context, IntPtr jsClass, JSNativeCallback data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSObjectHasProperty(IntPtr context, IntPtr obj, IntPtr propertyName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSObjectGetProperty(IntPtr context, IntPtr obj, IntPtr propertyName, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void JSObjectSetProperty(IntPtr context, IntPtr obj, IntPtr propertyName, IntPtr value, uint attributes, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSObjectDeleteProperty(IntPtr context, IntPtr obj, IntPtr propertyName, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSObjectGetPrivate(IntPtr obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void JSObjectSetPrivate(IntPtr obj, IntPtr data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSObjectIsFunction(IntPtr context, IntPtr obj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSObjectCallAsFunction(IntPtr context, IntPtr obj, IntPtr thisObject, ulong argumentCount, IntPtr arguments, IntPtr exception);

        // JSValueRef.h

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern int JSValueGetType(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsUndefined(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsNull(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsBoolean(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsNumber(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsString(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsObject(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsArray(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsEqual(IntPtr context, IntPtr a, IntPtr b, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueIsStrictEqual(IntPtr context, IntPtr a, IntPtr b);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueMakeUndefined(IntPtr context);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueMakeNull(IntPtr context);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueMakeBoolean(IntPtr context, bool boolean);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueMakeNumber(IntPtr context, double number);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueMakeString(IntPtr context, IntPtr str);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool JSValueToBoolean(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern double JSValueToNumber(IntPtr context, IntPtr value, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueToStringCopy(IntPtr context, IntPtr value, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueToObject(IntPtr context, IntPtr value, IntPtr exception);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueProtect(IntPtr context, IntPtr value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern IntPtr JSValueUnprotect(IntPtr context, IntPtr value);

    }
}
