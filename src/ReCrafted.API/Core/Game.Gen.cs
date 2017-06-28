// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 06/28/2017 12:37:39 Source: 'GameMain.API.cpp' Target: 'Core/Game.Gen.cs'

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
	/// <summary>
	///	Game class.
	/// </summary>
	public abstract partial class Game 
	{
		protected abstract void Initialize();

		protected abstract void Update();

		protected abstract void Simulate();

		protected abstract void Draw();

		protected abstract void DrawUI();

		protected abstract void Shutdown();

	}
}
