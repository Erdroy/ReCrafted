// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/01/2017 21:54:32 Source: 'Input.API.cpp' Target: 'Common/Input.Gen.cs'

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
		public static Vector2 GetCursorPosition 
		{
			get
			{
				return Internal_GetCursorPosition_Get();
			}
		}

		/// <summary>
		///	Returns cursor's position change since last frame.
		/// </summary>
		public static Vector2 GetCursorDelta 
		{
			get
			{
				return Internal_GetCursorDelta_Get();
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Vector2 Internal_GetCursorPosition_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Vector2 Internal_GetCursorDelta_Get();
	}
}
