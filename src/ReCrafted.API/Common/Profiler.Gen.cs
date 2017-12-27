// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
<<<<<<< HEAD
// Generated: 12/27/2017 12:20:39 Source: 'Profiler.API.cpp' Target: 'Common/Profiler.Gen.cs'
=======
// Generated: 12/27/2017 16:04:59 Source: 'Profiler.API.cpp' Target: 'Common/Profiler.Gen.cs'
>>>>>>> e4fda2a82ac2e0013f027ba59a31ec5acc89a5c5

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
