// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/20/2018 17:53:52 Source: 'InputManager.API.cpp' Target: 'Input/InputManager.Gen.cs'

using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Input 
{
	/// <summary>
	///	InputManager class.
	/// </summary>
	public static partial class InputManager 
	{
		/// <summary>
		///	Returns true when specified key is being held for at least one frame.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsKey(Key key);

		/// <summary>
		///	Returns true when specified key is has been pressed this frame.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsKeyDown(Key key);

		/// <summary>
		///	Returns true when specified key is has been released this frame.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsKeyUp(Key key);

		/// <summary>
		///	Returns true when specified button is being held for at least one frame.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsButton(Button button);

		/// <summary>
		///	Returns true when specified button is has been pressed this frame.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsButtonDown(Button button);

		/// <summary>
		///	Returns true when specified button is has been released this frame.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsButtonUp(Button button);

		/// <summary>
		///	Gets input device count.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern int GetDeviceCount();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern InputDevice GetDevice(int deviceId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern InputDevice GetDevice(DeviceType deviceType);

		/// <summary>
		///	Gets or sets the cursor position.
		/// </summary>
		public static Vector2 CursorPosition 
		{
			get
			{
				Vector2 result;
				Internal_CursorPosition_Get(out result);
				return result;
			}
			set
			{
				Internal_CursorPosition_Set(ref value);
			}
		}

		/// <summary>
		///	Gets cursor delta.
		/// </summary>
		public static Vector2 CursorDelta 
		{
			get
			{
				Vector2 result;
				Internal_CursorDelta_Get(out result);
				return result;
			}
		}

		/// <summary>
		///	Gets scroll delta.
		/// </summary>
		public static float ScrollDelta 
		{
			get
			{
				return Internal_ScrollDelta_Get();
			}
		}

		/// <summary>
		///	Gets or sets the cursor show state.
		/// </summary>
		public static bool ShowCursor 
		{
			get
			{
				return Internal_ShowCursor_Get();
			}
			set
			{
				Internal_ShowCursor_Set(value);
			}
		}

		/// <summary>
		///	Gets or sets the cursor lock state.
		/// </summary>
		public static bool LockCursor 
		{
			get
			{
				return Internal_LockCursor_Get();
			}
			set
			{
				Internal_LockCursor_Set(value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorPosition_Set(ref Vector2 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorPosition_Get(out Vector2 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorDelta_Get(out Vector2 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_ScrollDelta_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_ShowCursor_Set(bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_ShowCursor_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LockCursor_Set(bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_LockCursor_Get();
	}
}
