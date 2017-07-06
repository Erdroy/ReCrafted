// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/06/2017 16:38:19 Source: 'UI.API.cpp' Target: 'Common/Display.Gen.cs'

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
		public static int Width => Internal_Width_Get();

		/// <summary>
		///	Returns display's height
		/// </summary>
		public static int Height => Internal_Height_Get();

		/// <summary>
		///	Returns display's aspect ratio
		/// </summary>
		public static float AspectRatio => Internal_AspectRatio_Get();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Width_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Height_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_AspectRatio_Get();
	}
}
