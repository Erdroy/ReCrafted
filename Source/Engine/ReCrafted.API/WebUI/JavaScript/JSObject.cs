// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

using static ReCrafted.API.WebUI.JavaScript.JSCore;

namespace ReCrafted.API.WebUI.JavaScript
{
    public class JSObject
    {
        private readonly IntPtr _context;
        private readonly IntPtr _object;

        public JSObject(IntPtr context)
        {
            _context = context;
            _object = JSObjectMake(context, IntPtr.Zero, IntPtr.Zero);
            JSValueProtect(context, _object);
        }

        public JSObject(IntPtr context, IntPtr obj)
        {
            _context = context;
            _object = obj;
            JSValueProtect(context, _object);
        }

        ~JSObject()
        {
            JSValueUnprotect(_context, _object);
        }

        public JSValue GetPropertyValue(JSString propertyName)
        {
            var property = JSObjectGetProperty(_context, _object, propertyName.Get(), IntPtr.Zero);
            return property == IntPtr.Zero ? new JSValue(_context) : new JSValue(_context, property);
        }

        public void SetPropertyValue(JSString propertyName, JSValue value)
        {
            JSObjectSetProperty(_context, _object, propertyName.Get(), value.Get(), 2, IntPtr.Zero);
        }

        public bool HasProperty(JSString propertyName)
        {
            return JSObjectHasProperty(_context, _object, propertyName.Get());
        }

        public bool DeleteProperty(JSString propertyName)
        {
            return JSObjectDeleteProperty(_context, _object, propertyName.Get(), IntPtr.Zero);
        }

        public IntPtr GetContext()
        {
            return _context;
        }

        public IntPtr Get()
        {
            return _object;
        }

        public static JSObject GetGlobalObject(IntPtr context)
        {
            var obj = JSContextGetGlobalObject(context);
            return new JSObject(context, obj);
        }

        public static IntPtr GetGlobalContext(IntPtr context)
        {
            return JSContextGetGlobalContext(context);
        }
    }
}
