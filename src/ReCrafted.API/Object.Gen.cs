// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/01/2017 20:17:31 Source: 'Object.API.cpp' Target: 'Object.Gen.cs'

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
