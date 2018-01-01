// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/27/2017 12:20:39 Source: 'KeyboardBuffer.API.cpp' Target: 'Common/KeyboardBuffer.Gen.cs'

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
