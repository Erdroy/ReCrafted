// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/24/2017 20:56:33 Source: 'DebugDraw.API.cpp' Target: 'Graphics/DebugDraw.Gen.cs'

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	DebugDraw class.
	/// </summary>
	public static class DebugDraw 
	{
		/// <summary>
		///	Draws cube
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawCube(Vector3 center, Vector3 size);

		/// <summary>
		///	Draws wireframe cube
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawWireCube(Vector3 center, Vector3 size);

		/// <summary>
		///	Draws line
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DrawLine(Vector3 start, Vector3 end);

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

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Color_Set(ref Color value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Color_Get(out Color result);
	}
}
