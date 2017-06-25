// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 06/25/2017 16:23:46 Source: 'D:\ReCrafted\tools\..\src\ReCrafted\Core\Logger.API.cpp' Target: 'Core/Logger.Gen.cs'

using System;
using System.Linq;
using System.Collections.Generic;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Core 
{
	/// <summary>
	///	Logger class
	/// </summary>
	public partial class Logger : ILogger
	{
		/// <summary>
		///	Writes message to the output.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public static extern void Write(string message);

		/// <summary>
		///	Test method.
		/// </summary>
		public override void Test(string message, ref int test)
		{
		}

		private string Test2(string message, ref int test, out string outstr)
		{
			outstr = message + test.ToString();
			return outstr;
		}

	}
}
