// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/28/2018 15:22:27 Source: 'GameObject.API.cpp' Target: 'Common/GameObject.Gen.cs'

using ReCrafted.API.Common.Components;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	public sealed partial class GameObject : Object
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern unsafe TransformComponent* Internal_GetTransform(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AddScript(IntPtr nativePtr, IntPtr nativeScriptPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_RemoveScript(IntPtr nativePtr, IntPtr nativeScriptPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AddChildren(IntPtr nativePtr, IntPtr gameObjectPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_RemoveChildren(IntPtr nativePtr, IntPtr gameObjectPtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_GetChildCount(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern GameObject Internal_GetChild(IntPtr nativePtr, int index);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern uint Internal_GetEntityId(IntPtr nativePtr);

	}
}
