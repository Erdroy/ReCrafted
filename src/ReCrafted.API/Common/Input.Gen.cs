// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 02/22/2018 22:43:15 Source: 'Input.API.cpp' Target: 'Common/Input.Gen.cs'

using ReCrafted.API.Core;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	Input class.
	/// </summary>
	public static class Input 
	{
		/// <summary>
		///	Checks if the key is pressed down.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsKeyDown(Keys key);

		/// <summary>
		///	Checks if the key is not pressed.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsKeyUp(Keys key);

		/// <summary>
		///	Checks if the key held.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern bool IsKey(Keys key);

		/// <summary>
		///	Returns cursor's current position.
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
		///	Returns cursor's position change since last frame.
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
		///	Returns current scroll (mouse wheel) delta.
		/// </summary>
		public static float ScrollDelta 
		{
			get
			{
				return Internal_ScrollDelta_Get();
			}
		}

		/// <summary>
		///	Gets or sets the cursor lock state
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

		/// <summary>
		///	Gets or sets the cursor visibility state
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

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorPosition_Set(ref Vector2 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorPosition_Get(out Vector2 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorDelta_Get(out Vector2 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_ScrollDelta_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LockCursor_Set(bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_LockCursor_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_ShowCursor_Set(bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_ShowCursor_Get();
	}
}
