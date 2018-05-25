// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 05/25/2018 19:14:55 Source: 'WebUIView.API.cpp' Target: 'Graphics/WebUIView.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
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
		
	}
}
