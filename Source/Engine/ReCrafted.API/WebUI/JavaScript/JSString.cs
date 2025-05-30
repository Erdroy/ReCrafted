﻿// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Text;

using static ReCrafted.API.WebUI.JavaScript.JSCore;

namespace ReCrafted.API.WebUI.JavaScript
{
    public class JSString
    {
        private readonly IntPtr _string;

        public unsafe JSString(string str)
        {
            var data = Encoding.UTF8.GetBytes(str);
            fixed (byte* bytes = data)
            {
                _string = JSStringCreateWithUTF8CString(bytes);
            }
        }

        public JSString(IntPtr str)
        {
            _string = str;
        }

        ~JSString()
        {
            JSStringRelease(_string);
        }

        public JSString Copy()
        {
            return new JSString(JSStringRetain(_string));
        }

        public int Length()
        {
            return (int)JSStringGetLength(_string);
        }

        public void Release()
        {
            JSStringRelease(_string);
        }

        public IntPtr Get()
        {
            return _string;
        }

        public override bool Equals(object obj)
        {
            if (obj is JSString other)
            {
                return JSStringIsEqual(_string, other._string);
            }

            return false;
        }

        protected bool Equals(JSString other)
        {
            return JSStringIsEqual(_string, other._string);
        }

        public override int GetHashCode()
        {
            return _string.GetHashCode();
        }

        public override string ToString()
        {
            var len = Length() + 1;
            var data = new byte[len];

            unsafe
            {
                fixed (byte* bytes = data)
                {
                    JSStringGetUTF8CString(_string, bytes, (ulong)len);
                }
            }

            return Encoding.UTF8.GetString(data).TrimEnd();
        }
    }
}
