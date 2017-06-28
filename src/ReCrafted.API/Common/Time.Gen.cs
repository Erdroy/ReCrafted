// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 06/28/2017 16:24:01 Source: 'Time.API.cpp' Target: 'Common/Time.Gen.cs'

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	Time class.
	/// </summary>
	public static class Time 
	{
		/// <summary>
		///	Returns time between current and last frame.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern double DeltaTime();

		/// <summary>
		///	Returns current time
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern float CurrentTime();

	}
}
