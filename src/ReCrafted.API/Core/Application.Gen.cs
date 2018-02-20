// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 02/20/2018 19:15:47 Source: 'Application.API.cpp' Target: 'Core/Application.Gen.cs'

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

	}
}
