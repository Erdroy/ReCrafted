// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/22/2018 14:33:01 Source: 'ComponentSystem.API.cpp' Target: 'Common/Entities/ComponentSystem.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common.Entities 
{
	/// <summary>
	///	ECS ComponentSystem class.
	/// </summary>
	public abstract partial class ComponentSystem : Object
	{
        internal ComponentSystem() {}

		/// <summary>
		///	Called when this System is being initialized.
		/// </summary>
		protected virtual void Initialize()
		{
		}

		/// <summary>
		///	Called when this System is being shutdown.
		/// </summary>
		protected virtual void Shutdown()
		{
		}

		/// <summary>
		///	Updates this System. This is called by world.
		/// </summary>
		protected abstract void Update();

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern unsafe Entity* GetEntities(IntPtr systemNativePtr, out int numEntities);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void RequireComponent(IntPtr systemNativePtr, ushort componentTypeId, bool nativeComponentId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void ExcludeComponent(IntPtr systemNativePtr, ushort componentTypeId, bool nativeComponentId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern ComponentSystem Create(IntPtr systemType);

	}
}
