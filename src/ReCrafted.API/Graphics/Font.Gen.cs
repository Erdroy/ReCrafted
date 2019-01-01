// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/22/2018 16:14:42 Source: 'Font.API.cpp' Target: 'Graphics/Font.Gen.cs'

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
		public uint Size 
		{
			get
			{
				return Internal_Size_Get(NativePtr);
			}
			set
			{
				Internal_Size_Set(NativePtr, value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Size_Set(IntPtr nativePtr, uint value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern uint Internal_Size_Get(IntPtr nativePtr);
	}
}
