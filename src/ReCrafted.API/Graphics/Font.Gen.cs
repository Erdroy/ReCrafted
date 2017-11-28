// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/28/2017 19:46:39 Source: 'Font.API.cpp' Target: 'Graphics/Font.Gen.cs'

using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	Font class.
	/// </summary>
	public partial class Font : Object
	{
		/// <summary>
		///	Loads font from file
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Font Load(string fileName, int fontSize);

		/// <summary>
		///	Measures size of given string.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern uint Internal_MeasureString(IntPtr nativePtr, string text, out Vector2 size);

		/// <summary>
		///	The size of this font.
		/// </summary>
		public static uint Size 
		{
			get
			{
				return Internal_Size_Get();
			}
			set
			{
				Internal_Size_Set(value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Size_Set(uint value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern uint Internal_Size_Get();
	}
}
