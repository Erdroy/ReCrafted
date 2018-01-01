// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/26/2017 14:37:08 Source: 'Texture2D.API.cpp' Target: 'Graphics/Texture2D.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	Texture2D class.
	/// </summary>
	public partial class Texture2D : Object
	{
        internal Texture2D() {}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalLoadFile(IntPtr nativePtr, string fileName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalApply(IntPtr nativePtr);

		/// <summary>
		///	Creates new Texture2D
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Texture2D Create();

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
