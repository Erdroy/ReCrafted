// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/22/2017 19:14:07 Source: 'Texture2D.API.cpp' Target: 'Graphics/Texture2D.Gen.cs'

using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	Texture2D class.
	/// </summary>
	public class Texture2D : Object
	{
		/// <summary>
		///	Returns texture's width
		/// </summary>
		public int Width 
		{
			get
			{
				return Internal_Width_Get(NativePtr);
			}
		}

		/// <summary>
		///	Returns texture's height
		/// </summary>
		public int Height 
		{
			get
			{
				return Internal_Height_Get(NativePtr);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Width_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Height_Get(IntPtr nativePtr);
	}
}
