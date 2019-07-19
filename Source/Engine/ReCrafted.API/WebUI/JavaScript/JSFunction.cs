// ReCrafted (c) 2016-2019 Always Too Late

using System;

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

        public JSValue Invoke(JSObject thisObject)
        {
            var valuePtr = JSObjectCallAsFunction(_context, _object, thisObject.Get(), 0, IntPtr.Zero, IntPtr.Zero);
            return new JSValue(_context, valuePtr);
        }
    }
}
