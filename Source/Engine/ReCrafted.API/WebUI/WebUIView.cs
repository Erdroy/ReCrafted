// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using ReCrafted.API.Common;
using ReCrafted.API.WebUI.JavaScript;

namespace ReCrafted.API.WebUI
{
    public delegate JSValue WebUIViewFunction(JSObject thisObject, params JSValue[] args);

    public sealed partial class WebUIView
    {
        private readonly Dictionary<string, JSCallback> _callbacks = new Dictionary<string, JSCallback>();
        private readonly Dictionary<string, WebUIViewFunction> _functions = new Dictionary<string, WebUIViewFunction>();

        private JSObject _globalObject;

        /// <summary>
        ///     Binds callback with given name.
        /// </summary>
        /// <param name="name">The callback name. This is the name, of JS function object that will be created.</param>
        /// <param name="callback">The callback delegate.</param>
        public void BindCallback(string name, JSCallback callback)
        {
            if (_callbacks.ContainsKey(name))
            {
                Logger.LogWarning($"Callback with name '{name}' is already bound. Cannot bind.");
                return;
            }

            _callbacks.Add(name, callback);

            var global = JSObject.GetGlobalObject(Context);
            global.SetPropertyValue(new JSString(name), new JSValue(Context, callback));
        }

        /// <summary>
        ///     Unbinds callback of given name.
        /// </summary>
        /// <param name="name">The callback name that will be unbound.</param>
        public void UnbindCallback(string name)
        {
            if (!_callbacks.ContainsKey(name))
            {
                Logger.LogWarning($"Callback with name '{name}' is not bound. Cannot unbind.");
                return;
            }

            var global = JSObject.GetGlobalObject(Context);
            global.DeleteProperty(new JSString(name));

            _callbacks.Remove(name);
        }

        /// <summary>
        ///     Binds function of given name.
        /// </summary>
        /// <param name="name">The JS function name.</param>
        /// <returns>The function invocation delegate.</returns>
        public WebUIViewFunction BindFunction(string name)
        {
            if (_functions.ContainsKey(name))
            {
                Logger.LogWarning($"Function with name '{name}' is already bound. Cannot bind.");
                return null;
            }

            // Find function object
            var global = JSObject.GetGlobalObject(Context);
            var functionObject = global.GetPropertyValue(new JSString(name));
            var function = functionObject.ToFunction();

            // Get invoke delegate
            WebUIViewFunction invokeDelegate = function.Invoke;

            // Register and return the delegate
            _functions.Add(name, invokeDelegate);
            return invokeDelegate;
        }

        /// <summary>
        ///     Unbinds function of given name.
        /// </summary>
        /// <param name="name">The JS function name that will be unbound.</param>
        public void UnbindFunction(string name)
        {
            if (!_functions.ContainsKey(name))
            {
                Logger.LogWarning($"Function with name '{name}' is not bound. Cannot unbind.");
                return;
            }

            _functions.Remove(name);
        }

        /// <summary>
        ///     Converts value to JSValue object.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The JSValue object.</returns>
        public JSValue ToValue(string value)
        {
            return new JSValue(Context, value);
        }

        /// <summary>
        ///     Converts value to JSValue object.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The JSValue object.</returns>
        public JSValue ToValue(bool value)
        {
            return new JSValue(Context, value);
        }

        /// <summary>
        ///     Converts value to JSValue object.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The JSValue object.</returns>
        public JSValue ToValue(int value)
        {
            return new JSValue(Context, value);
        }

        /// <summary>
        ///     Converts value to JSValue object.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The JSValue object.</returns>
        public JSValue ToValue(uint value)
        {
            return new JSValue(Context, value);
        }

        /// <summary>
        ///     Converts value to JSValue object.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The JSValue object.</returns>
        public JSValue ToValue(long value)
        {
            return new JSValue(Context, value);
        }

        /// <summary>
        ///     Converts value to JSValue object.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The JSValue object.</returns>
        public JSValue ToValue(ulong value)
        {
            return new JSValue(Context, value);
        }

        /// <summary>
        ///     Converts value to JSValue object.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The JSValue object.</returns>
        public JSValue ToValue(float value)
        {
            return new JSValue(Context, value);
        }

        /// <summary>
        ///     Converts value to JSValue object.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The JSValue object.</returns>
        public JSValue ToValue(double value)
        {
            return new JSValue(Context, value);
        }

        /// <summary>
        ///     Gets the JS object instance of this View.
        /// </summary>
        public JSObject Object => _globalObject ?? (_globalObject = JSObject.GetGlobalObject(Context));

        public event Action BeginLoading
        {
            add => AddOnBeginLoading(value);
            remove => RemoveOnBeginLoading(value);
        }

        public event Action FinishLoading
        {
            add => AddOnFinishLoading(value);
            remove => RemoveOnFinishLoading(value);
        }

        public event Action DOMReady
        {
            add => AddOnDOMReady(value);
            remove => RemoveOnDOMReady(value);
        }
    }
}
