// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/15/2017 19:27:36 Source: 'Object.API.cpp' Target: 'Core/Logger.Gen.cs'

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
	/// <summary>
	///	Logger class. Writes debug messages to output file.
	/// </summary>
	public static partial class Logger 
	{
		/// <summary>
		///	Writes message to the output file.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Write(string message, LogLevel level);

	}
}
