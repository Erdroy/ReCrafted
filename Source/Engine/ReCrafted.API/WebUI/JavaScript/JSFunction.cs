// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Linq;
using static ReCrafted.API.WebUI.JavaScript.JSCore;

namespace ReCrafted.API.WebUI.JavaScript
{
    public class JSFunction
    {
        private readonly IntPtr _context;
        private readonly IntPtr _object;

        public JSFunction(IntPtr context, IntPtr obj)
        {
            _context = context;
            _object = obj;
        }

        public JSValue Invoke(JSObject thisObject, params JSValue[] arguments)
        {
            unsafe
            {
                var values = arguments.Select(x => x.Get()).ToArray();

                fixed (IntPtr* ptr = values)
                {
                    var valuePtr = JSObjectCallAsFunction(_context, _object, thisObject.Get(), (ulong)arguments.Length, new IntPtr(ptr), IntPtr.Zero);
                    return new JSValue(_context, valuePtr);
                }
            }
        }
    }
}
