// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/22/2018 14:51:47 Source: 'Texture.API.cpp' Target: 'Graphics/Texture.Gen.cs'

using ReCrafted.API.Content;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	Texture class.
	/// </summary>
	public partial class Texture : Asset
	{
        internal Texture() {}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalLoadFile(IntPtr nativePtr, string fileName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalApply(IntPtr nativePtr);

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
