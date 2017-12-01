// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/01/2017 14:58:16 Source: 'KeyboardBuffer.API.cpp' Target: 'Common/KeyboardBuffer.Gen.cs'

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
