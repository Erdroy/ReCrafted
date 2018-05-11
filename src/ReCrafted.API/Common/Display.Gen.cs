// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 05/11/2018 21:54:47 Source: 'Display.API.cpp' Target: 'Common/Display.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	Display class.
	/// </summary>
	public static class Display 
	{
		/// <summary>
		///	Returns display's width
		/// </summary>
		public static int Width 
		{
			get
			{
				return Internal_Width_Get();
			}
		}

		/// <summary>
		///	Returns display's height
		/// </summary>
		public static int Height 
		{
			get
			{
				return Internal_Height_Get();
			}
		}

		/// <summary>
		///	Returns display's aspect ratio
		/// </summary>
		public static float AspectRatio 
		{
			get
			{
				return Internal_AspectRatio_Get();
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Width_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Height_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_AspectRatio_Get();
	}
}
