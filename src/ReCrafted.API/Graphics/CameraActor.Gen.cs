// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/18/2018 17:33:05 Source: 'CameraActor.API.cpp' Target: 'Graphics/CameraActor.Gen.cs'

using ReCrafted.API.Common.Actors;
using ReCrafted.API.Graphics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	CameraActor actor.
	/// </summary>
	public partial class CameraActor : ActorBase
	{
        internal CameraActor() {}

		/// <summary>
		///	The camera instance used by this actor.
		/// </summary>
		public Camera Camera 
		{
			get
			{
				return Internal_Camera_Get(NativePtr);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Camera Internal_Camera_Get(IntPtr nativePtr);
	}
}
