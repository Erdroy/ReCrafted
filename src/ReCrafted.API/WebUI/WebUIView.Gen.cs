// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 01/07/2019 15:25:43 Source: 'WebUIView.API.cpp' Target: 'WebUI/WebUIView.Gen.cs'

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
		///	Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!
		/// </summary>
		public void Bind(string name, Action function)
		{
			Internal_Bind(NativePtr, name, function);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Bind(IntPtr nativePtr, string name, Action function);

		/// <summary>
		///	Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!
		/// </summary>
		public void Bind<T1>(string name, Action<T1> function)
		{
			Internal_Bind<T1>(NativePtr, name, function);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Bind<T1>(IntPtr nativePtr, string name, Action<T1> function);

		/// <summary>
		///	Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!
		/// </summary>
		public void Bind<T1, T2>(string name, Action<T1, T2> function)
		{
			Internal_Bind<T1, T2>(NativePtr, name, function);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Bind<T1, T2>(IntPtr nativePtr, string name, Action<T1, T2> function);

		/// <summary>
		///	Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!
		/// </summary>
		public void Bind<T1, T2, T3>(string name, Action<T1, T2, T3> function)
		{
			Internal_Bind<T1, T2, T3>(NativePtr, name, function);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Bind<T1, T2, T3>(IntPtr nativePtr, string name, Action<T1, T2, T3> function);

		/// <summary>
		///	Binds delegate as JavaScript function of given name. Note: Bind supports only: bool, string and float types!
		/// </summary>
		public void Bind<T1, T2, T3, T4>(string name, Action<T1, T2, T3, T4> function)
		{
			Internal_Bind<T1, T2, T3, T4>(NativePtr, name, function);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Bind<T1, T2, T3, T4>(IntPtr nativePtr, string name, Action<T1, T2, T3, T4> function);

		/// <summary>
		///	Creates new WebUIView
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern WebUIView Create();

		/// <summary>
		///	Gets or sets the active state.
		/// </summary>
		public bool Active 
		{
			get
			{
				return Internal_Active_Get(NativePtr);
			}
			set
			{
				Internal_Active_Set(NativePtr, value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Active_Set(IntPtr nativePtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_Active_Get(IntPtr nativePtr);
	}
}
