// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/19/2017 18:23:45 Source: 'UI.API.cpp' Target: 'UI/UI.Gen.cs'

using ReCrafted.API.Mathematics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.UI 
{
	/// <summary>
	///	UI internal class.
	/// </summary>
	internal static class UIInternal 
	{
		/// <summary>
		///	Draws box.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void DrawBox(RectangleF rect);

		/// <summary>
		///	The current drawing color
		/// </summary>
		internal static Color Color 
		{
			get
			{
				return Internal_Color_Get();
			}
			set
			{
				Internal_Color_Set(value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Color_Set(Color value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Color Internal_Color_Get();
	}
}
