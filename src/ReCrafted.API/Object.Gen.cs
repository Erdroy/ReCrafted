// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 01/09/2019 15:58:42 Source: 'Object.API.cpp' Target: 'Object.Gen.cs'

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
		internal static extern Object InternalNew(IntPtr managedType);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Object InternalNewGeneric(IntPtr baseType, object genericObject);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalDestroy(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalObjectFinalized(IntPtr nativePtr);

	}
}
