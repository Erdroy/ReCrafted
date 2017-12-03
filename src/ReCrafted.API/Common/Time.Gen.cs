// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/03/2017 19:00:15 Source: 'Time.API.cpp' Target: 'Common/Time.Gen.cs'

using System;
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
		public static double DeltaTime 
		{
			get
			{
				return Internal_DeltaTime_Get();
			}
		}

		/// <summary>
		///	Time between each simulation calls.
		/// </summary>
		public static double FixedDeltaTime 
		{
			get
			{
				return Internal_FixedDeltaTime_Get();
			}
			set
			{
				Internal_FixedDeltaTime_Set(value);
			}
		}

		/// <summary>
		///	Current time.
		/// </summary>
		public static double CurrentTime 
		{
			get
			{
				return Internal_CurrentTime_Get();
			}
		}

		/// <summary>
		///	The current game simulation time.
		/// </summary>
		public static double CurrentFixedTime 
		{
			get
			{
				return Internal_CurrentFixedTime_Get();
			}
		}

		/// <summary>
		///	Current frame count
		/// </summary>
		public static int Frames 
		{
			get
			{
				return Internal_Frames_Get();
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern double Internal_DeltaTime_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_FixedDeltaTime_Set(double value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern double Internal_FixedDeltaTime_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern double Internal_CurrentTime_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern double Internal_CurrentFixedTime_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Frames_Get();
	}
}
