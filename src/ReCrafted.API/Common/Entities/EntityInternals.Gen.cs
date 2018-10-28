// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/28/2018 14:37:57 Source: 'Entity.API.cpp' Target: 'Common/Entities/EntityInternals.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common.Entities 
{
	internal static class EntityInternals 
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void ActivateEntity(IntPtr world, uint entityId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern unsafe void AddEntityComponent(IntPtr world, uint entityId, void* componentData, uint componentSize, ushort componentId, bool nativeComponentId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern unsafe ComponentData* GetEntityComponent(IntPtr world, uint entityId, ushort componentId, bool nativeComponentId);

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
