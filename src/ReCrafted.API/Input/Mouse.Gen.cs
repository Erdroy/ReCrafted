// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/21/2018 13:55:44 Source: 'Mouse.API.cpp' Target: 'Input/Mouse.Gen.cs'

using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Input 
{
	/// <summary>
	///	Mouse input device
	/// </summary>
	public partial class Mouse : InputDevice
	{
        internal Mouse() {}

		/// <summary>
		///	Returns true when specified button is being held for at least one frame.
		/// </summary>
		public bool IsButton(Button button)
		{
			return Internal_IsButton(NativePtr, button);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsButton(IntPtr nativePtr, Button button);

		/// <summary>
		///	Returns true when specified button is has been pressed this frame.
		/// </summary>
		public bool IsButtonDown(Button button)
		{
			return Internal_IsButtonDown(NativePtr, button);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsButtonDown(IntPtr nativePtr, Button button);

		/// <summary>
		///	Returns true when specified button is has been released this frame.
		/// </summary>
		public bool IsButtonUp(Button button)
		{
			return Internal_IsButtonUp(NativePtr, button);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsButtonUp(IntPtr nativePtr, Button button);

		/// <summary>
		///	Gets or sets the cursor position.
		/// </summary>
		public Vector2 CursorPosition 
		{
			get
			{
				Vector2 result;
				Internal_CursorPosition_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_CursorPosition_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets cursor delta.
		/// </summary>
		public Vector2 CursorDelta 
		{
			get
			{
				Vector2 result;
				Internal_CursorDelta_Get(NativePtr, out result);
				return result;
			}
		}

		/// <summary>
		///	Gets scroll delta.
		/// </summary>
		public float ScrollDelta 
		{
			get
			{
				return Internal_ScrollDelta_Get(NativePtr);
			}
		}

		/// <summary>
		///	Gets or sets the cursor show state.
		/// </summary>
		public bool ShowCursor 
		{
			get
			{
				return Internal_ShowCursor_Get(NativePtr);
			}
			set
			{
				Internal_ShowCursor_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets the cursor lock state.
		/// </summary>
		public bool LockCursor 
		{
			get
			{
				return Internal_LockCursor_Get(NativePtr);
			}
			set
			{
				Internal_LockCursor_Set(NativePtr, value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorPosition_Set(IntPtr nativePtr, ref Vector2 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorPosition_Get(IntPtr nativePtr, out Vector2 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorDelta_Get(IntPtr nativePtr, out Vector2 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_ScrollDelta_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_ShowCursor_Set(IntPtr nativePtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_ShowCursor_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LockCursor_Set(IntPtr nativePtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_LockCursor_Get(IntPtr nativePtr);
	}
}
