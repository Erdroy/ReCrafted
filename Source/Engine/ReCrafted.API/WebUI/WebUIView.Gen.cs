// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using System.Runtime.CompilerServices;

namespace ReCrafted.API.WebUI 
{
    /// <summary>
    ///     WebUIView class. Provides WebUI view rendering and interaction.
    /// </summary>
    public sealed partial class WebUIView : Object
    {
        
        /// <summary>
        ///     Resizes the view port.
        /// </summary>
        public void Resize(uint width, uint height)
        {
            InternalResize(NativePtr, width, height);
        }
        
        /// <summary>
        ///     Navigates this view to given URL.
        /// </summary>
        /// <param name="url">The URL to navigate to.</param>
        public void Navigate(string url)
        {
            InternalNavigate(NativePtr, url);
        }
        
        /// <summary>
        ///     Executes given JS source code in the view's JS context.
        /// </summary>
        /// <param name="javaScriptSource">The JS source code.</param>
        public void Execute(string javaScriptSource)
        {
            InternalExecute(NativePtr, javaScriptSource);
        }
        
        /// <summary>
        ///     Binds raw callback with no parameters.
        /// </summary>
        /// <param name="functionName">The target function name.</param>
        /// <param name="callback">The callback that will be invoked when JS calls function of given name.</param>
        public void BindCallback(string functionName, System.Action callback)
        {
            InternalBindCallback(NativePtr, functionName, callback);
        }
        
        private void AddOnBeginLoading(System.Action action)
        {
            InternalAddOnBeginLoading(NativePtr, action);
        }
        
        private void RemoveOnBeginLoading(System.Action action)
        {
            InternalRemoveOnBeginLoading(NativePtr, action);
        }
        
        private void AddOnFinishLoading(System.Action action)
        {
            InternalAddOnFinishLoading(NativePtr, action);
        }
        
        private void RemoveOnFinishLoading(System.Action action)
        {
            InternalRemoveOnFinishLoading(NativePtr, action);
        }
        
        private void AddOnDOMReady(System.Action action)
        {
            InternalAddOnDOMReady(NativePtr, action);
        }
        
        private void RemoveOnDOMReady(System.Action action)
        {
            InternalRemoveOnDOMReady(NativePtr, action);
        }

        /// <summary>
        ///     Gets or sets the view's active state. When false, this view will not be processed and rendered.
        /// </summary>
        public bool Active
        {
            get
            {
                Get_InternalActive(NativePtr, out var data);
                return data;
            }
            set => Set_InternalActive(NativePtr, ref value);
        }

        /// <summary>
        ///     Gets or sets the view's name.
        /// </summary>
        public string Name
        {
            get
            {
                Get_InternalName(NativePtr, out var data);
                return data;
            }
            set => Set_InternalName(NativePtr, ref value);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalResize(System.IntPtr nativePtr, uint width, uint height);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalNavigate(System.IntPtr nativePtr, string url);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalExecute(System.IntPtr nativePtr, string javaScriptSource);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalBindCallback(System.IntPtr nativePtr, string functionName, System.Action callback);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddOnBeginLoading(System.IntPtr nativePtr, System.Action action);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalRemoveOnBeginLoading(System.IntPtr nativePtr, System.Action action);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddOnFinishLoading(System.IntPtr nativePtr, System.Action action);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalRemoveOnFinishLoading(System.IntPtr nativePtr, System.Action action);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddOnDOMReady(System.IntPtr nativePtr, System.Action action);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalRemoveOnDOMReady(System.IntPtr nativePtr, System.Action action);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalActive(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalActive(System.IntPtr nativePtr, ref bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalName(System.IntPtr nativePtr, out string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalName(System.IntPtr nativePtr, ref string data);
    }
}