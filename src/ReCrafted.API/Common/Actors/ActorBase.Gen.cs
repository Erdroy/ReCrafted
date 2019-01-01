// ReCrafted (c) 2016-2019 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/18/2018 17:12:14 Source: 'ActorBase.API.cpp' Target: 'Common/Actors/ActorBase.Gen.cs'

using ReCrafted.API.Common;
using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common.Actors 
{
	/// <summary>
	///	ActorBase class.
	/// </summary>
	public partial class ActorBase : Object
	{
        internal ActorBase() {}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Transform Internal_GetTransform(IntPtr nativePtr);

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_SetTransform(IntPtr nativePtr, ref Transform transform);

		/// <summary>
		///	Gets child from this actor with given index.
		/// </summary>
		public ActorBase GetChild(int index)
		{
			return Internal_GetChild(NativePtr, index);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern ActorBase Internal_GetChild(IntPtr nativePtr, int index);

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

		/// <summary>
		///	Destroys this actor. Actor will be released at the end of the current frame.
		/// </summary>
		public new void Destroy()
		{
			Internal_Destroy(NativePtr);
		}
        
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Destroy(IntPtr nativePtr);

		/// <summary>
		///	Gets or sets current actor position.
		/// </summary>
		public Vector3 Position 
		{
			get
			{
				Vector3 result;
				Internal_Position_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_Position_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets or sets current actor position.
		/// </summary>
		public Vector3 LocalPosition 
		{
			get
			{
				Vector3 result;
				Internal_LocalPosition_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_LocalPosition_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets or sets current actor rotation.
		/// </summary>
		public Quaternion Rotation 
		{
			get
			{
				Quaternion result;
				Internal_Rotation_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_Rotation_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets or sets current actor rotation.
		/// </summary>
		public Quaternion LocalRotation 
		{
			get
			{
				Quaternion result;
				Internal_LocalRotation_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_LocalRotation_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets or sets current actor scale.
		/// </summary>
		public Vector3 Scale 
		{
			get
			{
				return Internal_Scale_Get(NativePtr);
			}
			set
			{
				Internal_Scale_Set(NativePtr, value);
			}
		}

		/// <summary>
		///	Gets or sets current actor scale.
		/// </summary>
		public Vector3 LocalScale 
		{
			get
			{
				Vector3 result;
				Internal_LocalScale_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_LocalScale_Set(NativePtr, ref value);
			}
		}

		/// <summary>
		///	Gets children count of this actor.
		/// </summary>
		public int ChildCount 
		{
			get
			{
				return Internal_ChildCount_Get(NativePtr);
			}
		}

		/// <summary>
		///	Activates or deactivates this actor.
		/// </summary>
		public bool IsActive 
		{
			get
			{
				bool result;
				Internal_IsActive_Get(NativePtr, out result);
				return result;
			}
			set
			{
				Internal_IsActive_Set(NativePtr, ref value);
			}
		}

		public bool IsActiveSelf 
		{
			get
			{
				return Internal_IsActiveSelf_Get(NativePtr);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Position_Set(IntPtr nativePtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Position_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LocalPosition_Set(IntPtr nativePtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LocalPosition_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Rotation_Set(IntPtr nativePtr, ref Quaternion value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Rotation_Get(IntPtr nativePtr, out Quaternion result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LocalRotation_Set(IntPtr nativePtr, ref Quaternion value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LocalRotation_Get(IntPtr nativePtr, out Quaternion result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Scale_Set(IntPtr nativePtr, Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Vector3 Internal_Scale_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LocalScale_Set(IntPtr nativePtr, ref Vector3 value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LocalScale_Get(IntPtr nativePtr, out Vector3 result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_ChildCount_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_IsActive_Set(IntPtr nativePtr, ref bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_IsActive_Get(IntPtr nativePtr, out bool result);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsActiveSelf_Get(IntPtr nativePtr);
	}
}
