// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/01/2017 21:54:32 Source: 'Logger.API.cpp' Target: 'Core/Logger.Gen.cs'

using System;
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
