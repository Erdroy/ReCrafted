// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Text;

using static ReCrafted.API.WebUI.JavaScript.JSCore;

namespace ReCrafted.API.WebUI.JavaScript
{
    public delegate JSValue JSCallback(JSObject thisObject, params JSValue[] args);

    public class JSValue
    {
        private readonly IntPtr _context;
        private readonly IntPtr _value;

        public JSValue(IntPtr context)
        {
            _context = context;
            _value = JSValueMakeUndefined(context);
            JSValueProtect(_context, _value);
        }

        public JSValue(IntPtr context, JSCallback callback)
        {
            _context = context;

            _value = JSObjectMakeCallback(_context, JSGetNativeCallbackClass(), (thisObject, nativeArgs) =>
            {
                if (nativeArgs != null)
                {
                    var args = new JSValue[nativeArgs.Length];
                    for (var i = 0; i < nativeArgs.Length; i++)
                        args[i] = new JSValue(_context, nativeArgs[i]);

                    return callback(new JSObject(_context, thisObject), args).Get();
                }

                return callback(new JSObject(_context, thisObject)).Get();
            });
            JSValueProtect(_context, _value);
        }

        public JSValue(IntPtr context, bool value)
        {
            _context = context;
            _value = JSValueMakeBoolean(context, value);
            JSValueProtect(_context, _value);
        }

        public JSValue(IntPtr context, int value)
        {
            _context = context;
            _value = JSValueMakeNumber(context, value);
            JSValueProtect(_context, _value);
        }

        public JSValue(IntPtr context, uint value)
        {
            _context = context;
            _value = JSValueMakeNumber(context, value);
            JSValueProtect(_context, _value);
        }

        public JSValue(IntPtr context, long value)
        {
            _context = context;
            _value = JSValueMakeNumber(context, value);
            JSValueProtect(_context, _value);
        }

        public JSValue(IntPtr context, ulong value)
        {
            _context = context;
            _value = JSValueMakeNumber(context, value);
            JSValueProtect(_context, _value);
        }

        public JSValue(IntPtr context, string value)
        {
            _context = context;
            var data = Encoding.UTF8.GetBytes(value);

            unsafe
            {
                fixed (byte* bytes = data)
                {
                    _value = JSValueMakeString(context, JSStringCreateWithUTF8CString(bytes));
                }
            }

            JSValueProtect(_context, _value);
        }

        public JSValue(IntPtr context, IntPtr value)
        {
            _context = context;
            _value = value;
            JSValueProtect(_context, value);
        }

        ~JSValue()
        {
            JSValueUnprotect(_context, _value);
        }

        public bool IsNull()
        {
            return JSValueIsNull(_context, _value);
        }

        public bool IsUndefined()
        {
            return JSValueIsUndefined(_context, _value);
        }

        public bool IsBoolean()
        {
            return JSValueIsBoolean(_context, _value);
        }

        public bool IsNumber()
        {
            return JSValueIsNumber(_context, _value);
        }

        public bool IsString()
        {
            return JSValueIsString(_context, _value);
        }

        public bool IsObject()
        {
            return JSValueIsObject(_context, _value);
        }

        public bool IsFunction()
        {
            if (!IsObject())
                return false;

            var obj = JSValueToObject(_context, _value, IntPtr.Zero);
            return JSObjectIsFunction(_context, obj);
        }

        public bool ToBoolean()
        {
            return JSValueToBoolean(_context, _value);
        }

        public double ToNumber()
        {
            return JSValueToNumber(_context, _value, IntPtr.Zero);
        }

        public long ToInteger()
        {
            return (long)JSValueToNumber(_context, _value, IntPtr.Zero);
        }

        public new JSString ToString()
        {
            var str = JSValueToStringCopy(_context, _value, IntPtr.Zero);
            return new JSString(str);
        }

        public IntPtr Get()
        {
            return _value;
        }

        public JSObject ToObject()
        {
            var obj = JSValueToObject(_context, _value, IntPtr.Zero);
            return new JSObject(_context, obj);
        }

        public JSFunction ToFunction()
        {
            var obj = JSValueToObject(_context, _value, IntPtr.Zero);

            if (!JSObjectIsFunction(_context, obj))
                return new JSFunction(_context, IntPtr.Zero);

            return new JSFunction(_context, obj);
        }

        public static JSValue Null => new JSValue(IntPtr.Zero, IntPtr.Zero);
    }
}
