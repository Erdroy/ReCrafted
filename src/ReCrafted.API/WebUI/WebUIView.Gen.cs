// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/28/2018 16:37:42 Source: 'WebUIView.API.cpp' Target: 'WebUI/WebUIView.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.WebUI 
{
	/// <summary>
	///	WebUIView class.
	/// </summary>
	public partial class WebUIView : Object
	{
        internal WebUIView() {}

		/// <summary>
		///	Navigates current view to given URL.
		/// </summary>
		public void Navigate(string url)
		{
			Internal_Navigate(NativePtr, url);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Navigate(IntPtr nativePtr, string url);

		/// <summary>
		///	Executes given JavaScript code.
		/// </summary>
		public void ExecuteJS(string javaScript)
		{
			Internal_ExecuteJS(NativePtr, javaScript);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_ExecuteJS(IntPtr nativePtr, string javaScript);

		/// <summary>
		///	Creates new WebUIView
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern WebUIView Create();

	}
}
