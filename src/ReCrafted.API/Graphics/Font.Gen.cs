// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/26/2017 16:33:00 Source: 'Font.API.cpp' Target: 'Graphics/Font.Gen.cs'

using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	Font class.
	/// </summary>
	public class Font : Object
	{
		/// <summary>
		///	Loads font from file
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Font Load(string fileName, int fontSize);

		/// <summary>
		///	Get size of font.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern uint GetSize(IntPtr nativePtr);

	}
}
