// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/21/2018 13:55:44 Source: 'Keyboard.API.cpp' Target: 'Input/Keyboard.Gen.cs'

using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Input 
{
	/// <summary>
	///	Keyboard input device
	/// </summary>
	public partial class Keyboard : InputDevice
	{
        internal Keyboard() {}

		/// <summary>
		///	Returns true when specified key is being held for at least one frame.
		/// </summary>
		public bool IsKey(Key key)
		{
			return Internal_IsKey(NativePtr, key);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsKey(IntPtr nativePtr, Key key);

		/// <summary>
		///	Returns true when specified key is has been pressed this frame.
		/// </summary>
		public bool IsKeyDown(Key key)
		{
			return Internal_IsKeyDown(NativePtr, key);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsKeyDown(IntPtr nativePtr, Key key);

		/// <summary>
		///	Returns true when specified key is has been released this frame.
		/// </summary>
		public bool IsKeyUp(Key key)
		{
			return Internal_IsKeyUp(NativePtr, key);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsKeyUp(IntPtr nativePtr, Key key);

		/// <summary>
		///	Contains character string that has been entered during current frame.
		/// </summary>
		public string InputString 
		{
			get
			{
				return Internal_InputString_Get(NativePtr);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern string Internal_InputString_Get(IntPtr nativePtr);
	}
}
