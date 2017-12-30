// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/30/2017 14:15:07 Source: 'Input.API.cpp' Target: 'Common/Input.Gen.cs'

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
		///	Changes cursor's current positon.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void SetCursorPosition(Vector2 position);

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

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorPosition_Get(out Vector2 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorDelta_Get(out Vector2 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_ScrollDelta_Get();
	}
}
