// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 05/25/2018 18:47:25 Source: 'Entity.API.cpp' Target: 'Common/Entity.Gen.cs'

using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	Entity class.
	/// </summary>
	public partial class Entity : Object
	{
        internal Entity() {}

		/// <summary>
		///	Creates new Entity
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern Entity Create(string entityName);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_AddScript(IntPtr nativePtr, Script instance);
		
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_RemoveScript(IntPtr nativePtr, Script instance);
		
		/// <summary>
		///	The position of this entity.
		/// </summary>
		public Vector3 Position 
		{
			get
			{
				return Internal_Position_Get(NativePtr);
			}
			set
			{
				Internal_Position_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	The rotation of this entity.
		/// </summary>
		public Vector3 Rotation 
		{
			get
			{
				return Internal_Rotation_Get(NativePtr);
			}
			set
			{
				Internal_Rotation_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	The guid of this entity.
		/// </summary>
		public Guid Guid 
		{
			get
			{
				return Internal_Guid_Get(NativePtr);
			}
		}

		/// <summary>
		///	The name of this entity.
		/// </summary>
		public string Name 
		{
			get
			{
				return Internal_Name_Get(NativePtr);
			}
			set
			{
				Internal_Name_Set(NativePtr, value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Position_Set(IntPtr nativePtr, Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Vector3 Internal_Position_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Rotation_Set(IntPtr nativePtr, Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Vector3 Internal_Rotation_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Guid Internal_Guid_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Name_Set(IntPtr nativePtr, string value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern string Internal_Name_Get(IntPtr nativePtr);
	}
}
