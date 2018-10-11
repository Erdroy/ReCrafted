// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/11/2018 13:51:16 Source: 'Object.API.cpp' Target: 'Object.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API 
{
	/// <summary>
	///	Base class for all mono object instances.
	/// </summary>
	public partial class Object 
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalDestroy(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalObjectFinalized(IntPtr nativePtr);

	}
}
