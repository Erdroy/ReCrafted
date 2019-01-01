// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/14/2018 18:07:50 Source: 'Profiler.API.cpp' Target: 'Common/Profiler.Gen.cs'

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
		public static extern void BeginProfile(string name);

		/// <summary>
		///	Ends profiling.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void EndProfile();

	}
}
