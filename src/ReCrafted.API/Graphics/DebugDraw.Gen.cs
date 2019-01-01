// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/06/2018 12:01:30 Source: 'DebugDraw.API.cpp' Target: 'Graphics/DebugDraw.Gen.cs'

using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	DebugDraw class.
	/// </summary>
	public static class DebugDraw 
	{
		/// <summary>
		///	Draws box using given position (center) and size.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawBox(Vector3 center, Vector3 size);

		/// <summary>
		///	Draws wire box using given position (center) and size.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawWireBox(Vector3 center, Vector3 size);

		/// <summary>
		///	Draws line
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawLine(Vector3 start, Vector3 end);

		/// <summary>
		///	Draws sphere at given position.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawSphere(Vector3 center, float radius);

		/// <summary>
		///	Draws wire sphere at given position.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawWireSphere(Vector3 center, float radius);

		/// <summary>
		///	Draws arrow at the end point with line.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawArrow(Vector3 start, Vector3 end, float arrowSize);

		/// <summary>
		///	Gets/Sets drawing color.
		/// </summary>
		public static Color Color 
		{
			get
			{
				Color result;
				Internal_Color_Get(out result);
				return result;
			}
			set
			{
				Internal_Color_Set(ref value);
			}
		}

		/// <summary>
		///	Gets/Sets transformation Matrix.
		/// </summary>
		public static Matrix Matrix 
		{
			get
			{
				Matrix result;
				Internal_Matrix_Get(out result);
				return result;
			}
			set
			{
				Internal_Matrix_Set(ref value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Color_Set(ref Color value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Color_Get(out Color result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Matrix_Set(ref Matrix value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Matrix_Get(out Matrix result);
	}
}
