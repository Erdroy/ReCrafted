// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/27/2017 16:04:59 Source: 'Profiler.API.cpp' Target: 'Common/Profiler.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	Profiler class.
	/// </summary>
	public static class Profiler 
	{
		/// <summary>
		///	Begins profiling.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void BeginProfile(string name, float warm = -1.0f, float max = -1.0f);

		/// <summary>
		///	Ends profiling.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void EndProfile();

	}
}
