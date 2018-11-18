// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/18/2018 17:10:25 Source: 'Script.API.cpp' Target: 'Common/Script.Gen.cs'

using ReCrafted.API.Common.Components;
using ReCrafted.API.Common.Actors;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	/// <summary>
	///	Script class
	/// </summary>
	public partial class Script : Object
	{
		/// <summary>
		///	The actor that contains this script.
		/// </summary>
		public ActorBase Actor 
		{
			get
			{
				return Internal_Actor_Get(NativePtr);
			}
		}

		/// <summary>
		///	The enable state of this script. True when enabled.
		/// </summary>
		public bool Enabled 
		{
			get
			{
				return Internal_Enabled_Get(NativePtr);
			}
			set
			{
				Internal_Enabled_Set(NativePtr, value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern ActorBase Internal_Actor_Get(IntPtr nativePtr);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Enabled_Set(IntPtr nativePtr, bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_Enabled_Get(IntPtr nativePtr);
	}
}
