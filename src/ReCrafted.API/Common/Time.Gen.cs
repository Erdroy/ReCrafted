// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/14/2017 16:19:19 Source: 'UI.API.cpp' Target: 'Common/Time.Gen.cs'

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	Time class.
	/// </summary>
	public static class Time 
	{
		/// <summary>
		///	Time between current and last frame.
		/// </summary>
		public static double DeltaTime => Internal_DeltaTime_Get();

		/// <summary>
		///	Current time.
		/// </summary>
		public static double CurrentTime => Internal_CurrentTime_Get();

		/// <summary>
		///	Current frame count
		/// </summary>
		public static int Frames => Internal_Frames_Get();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern double Internal_DeltaTime_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern double Internal_CurrentTime_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Frames_Get();
	}
}
