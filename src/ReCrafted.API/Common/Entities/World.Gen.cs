// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/16/2018 20:31:54 Source: 'World.API.cpp' Target: 'Common/Entities/World.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common.Entities 
{
	/// <summary>
	///	ECS World class.
	/// </summary>
	public partial class World : Object
	{
        internal World() {}

		/// <summary>
		///	Updates this World
		/// </summary>
		public void Update()
		{
			Internal_Update(NativePtr);
		}
        
        [MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Update(IntPtr nativePtr);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void AddSystem(IntPtr worldNativePtr, IntPtr systemNativePtr, ushort systemId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern void RemoveSystem(IntPtr worldNativePtr, ushort systemId);

		[MethodImpl(MethodImplOptions.InternalCall)]
		private static extern bool HasSystem(IntPtr worldNativePtr, ushort systemId);

		/// <summary>
		///	Creates new World
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Create();

		/// <summary>
		///	Creates new World
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern World GetMainWorld();

	}
}
