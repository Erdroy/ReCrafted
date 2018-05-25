// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 05/25/2018 19:14:55 Source: 'KeyboardBuffer.API.cpp' Target: 'Common/KeyboardBuffer.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	KeyboardBuffer class.
	/// </summary>
	public static class KeyboardBuffer 
	{
		/// <summary>
		///	Reads the keyboard buffer.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern char[] Read();

	}
}
