// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/24/2017 20:56:33 Source: 'Display.API.cpp' Target: 'Common/Display.Gen.cs'

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
