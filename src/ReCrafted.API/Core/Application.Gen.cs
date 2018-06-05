// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 06/04/2018 23:05:32 Source: 'Application.API.cpp' Target: 'Core/Application.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
	/// <summary>
	///	Application class.
	/// </summary>
	public abstract class Application 
	{
		protected abstract void Initialize();

		protected abstract void Shutdown();

		protected abstract void Update();

		protected abstract void Simulate();

		protected abstract void Render();

		protected abstract void RenderUI();

		/// <summary>
		///	The target amount of frames per second (FPS).
		/// </summary>
		public static int TargetFps 
		{
			get
			{
				return Internal_TargetFps_Get();
			}
			set
			{
				Internal_TargetFps_Set(value);
			}
		}

		/// <summary>
		///	Enables or disables vertical synchronization (vsync).
		/// </summary>
		public static bool VSync 
		{
			get
			{
				return Internal_VSync_Get();
			}
			set
			{
				Internal_VSync_Set(value);
			}
		}

		/// <summary>
		///	Gets or sets current cursor icon.
		/// </summary>
		public static int CursorIcon 
		{
			get
			{
				return Internal_CursorIcon_Get();
			}
			set
			{
				Internal_CursorIcon_Set(value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_TargetFps_Set(int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_TargetFps_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_VSync_Set(bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_VSync_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_CursorIcon_Set(int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_CursorIcon_Get();
	}
}
