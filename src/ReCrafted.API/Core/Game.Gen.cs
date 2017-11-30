// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/30/2017 11:38:31 Source: 'GameMain.API.cpp' Target: 'Core/Game.Gen.cs'

using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
	/// <summary>
	///	Game class.
	/// </summary>
	public abstract class Game 
	{
		protected abstract void Initialize();

		protected abstract void Update();

		protected abstract void Simulate();

		protected abstract void Draw();

		protected abstract void DrawUI();

		protected abstract void Shutdown();

		/// <summary>
		///	Quits the game.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Quit();

		/// <summary>
		///	The simulation tickrate. Default is 60
		/// </summary>
		public static int Tickrate 
		{
			get
			{
				return Internal_Tickrate_Get();
			}
			set
			{
				Internal_Tickrate_Set(value);
			}
		}

		/// <summary>
		///	Gets or sets the cursor state
		/// </summary>
		public static bool ShowCursor 
		{
			get
			{
				return Internal_ShowCursor_Get();
			}
			set
			{
				Internal_ShowCursor_Set(value);
			}
		}

		/// <summary>
		///	Gets or sets the cursor lock mode
		/// </summary>
		public static bool LockCursor 
		{
			get
			{
				return Internal_LockCursor_Get();
			}
			set
			{
				Internal_LockCursor_Set(value);
			}
		}

		/// <summary>
		///	Gets or sets the target fps. Set to 0 to set FPS Cap to unlimited.
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
		///	Is game running state.
		/// </summary>
		public static bool IsRunning 
		{
			get
			{
				return Internal_IsRunning_Get();
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Tickrate_Set(int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Tickrate_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_ShowCursor_Set(bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_ShowCursor_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_LockCursor_Set(bool value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_LockCursor_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_TargetFps_Set(int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_TargetFps_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsRunning_Get();
	}
}
