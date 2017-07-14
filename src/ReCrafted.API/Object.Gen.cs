// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/14/2017 16:19:19 Source: 'UI.API.cpp' Target: 'Object.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API 
{
	/// <summary>
	///	Base class for all mono object instances.
	/// </summary>
	public abstract partial class Object 
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalDestroy(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void InternalObjectFinalized(IntPtr nativePtr);

	}
}
