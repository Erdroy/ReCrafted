// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/10/2018 14:33:14 Source: 'ActorBase.API.cpp' Target: 'Common/ActorBase.Gen.cs'

using ReCrafted.API.Common;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	ActorBase class.
	/// </summary>
	public partial class ActorBase : Object
	{
        internal ActorBase() {}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern unsafe Transform* Internal_GetTransform(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AddScript(IntPtr nativePtr, IntPtr nativeScriptPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_RemoveScript(IntPtr nativePtr, IntPtr nativeScriptPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_SetParent(IntPtr nativePtr, IntPtr actorPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AddChild(IntPtr nativePtr, IntPtr actorPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_RemoveChild(IntPtr nativePtr, IntPtr actorPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_GetChildCount(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern ActorBase Internal_GetChild(IntPtr nativePtr, int index);

	}
}
