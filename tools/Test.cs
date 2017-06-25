// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 06/25/2017 16:06:54 Source: 'Logger.API.cpp' Target: 'Core/Logger.Generated.cs'

using System;
using System.Linq;
using System.Collections.Generic;

namespace ReCrafted.API 
{
	/// <summary>
	///	Logger base class
	/// </summary>
	public abstract class ILogger 
	{
		/// <summary>
		///	Test method.
		/// </summary>
		public abstract void Test(string message, ref int Test, out string outstr);

	}
}
namespace ReCrafted.API 
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
		public override void Test(string message, ref int Test)
		{
		}

		private string Test2(string message, ref int test, out string outstr)
		{
			outstr = message + test.ToString();
			return outstr;
		}

	}
}
