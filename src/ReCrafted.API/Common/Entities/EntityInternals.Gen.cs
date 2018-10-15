// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/13/2018 13:25:48 Source: 'Entity.API.cpp' Target: 'Common/Entities/EntityInternals.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common.Entities 
{
	internal static class EntityInternals 
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void ActivateEntity(IntPtr world, uint entityId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void AddEntityComponent(IntPtr world, uint entityId, IntPtr componentData, uint componentSize, ushort componentId, bool nativeComponentId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void GetEntityComponent(IntPtr world, uint entityId, ushort componentId, bool nativeComponentId, IntPtr managedDataPtr, uint componentSize);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void RemoveEntityComponent(IntPtr world, uint entityId, ushort componentId, bool nativeComponentId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DeactivateEntity(IntPtr world, uint entityId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void DestroyEntity(IntPtr world, uint entityId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void CleanEntity(IntPtr world, uint entityId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Entity CreateEntity(IntPtr world);

	}
}
