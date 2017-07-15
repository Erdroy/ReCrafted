// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/15/2017 19:27:36 Source: 'Object.API.cpp' Target: 'Core/Game.Gen.cs'

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
			get { return Internal_Tickrate_Get(); }
			set { Internal_Tickrate_Set(value); }
		}

		/// <summary>
		///	Is game running state.
		/// </summary>
		public static bool IsRunning => Internal_IsRunning_Get();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Tickrate_Set(int value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern int Internal_Tickrate_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern bool Internal_IsRunning_Get();
	}
}
