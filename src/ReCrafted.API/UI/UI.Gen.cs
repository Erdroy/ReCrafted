// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 11/01/2017 20:17:30 Source: 'UI.API.cpp' Target: 'UI/UI.Gen.cs'

using ReCrafted.API.Mathematics;
using ReCrafted.API.Graphics;
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
		///	Draws string.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void DrawString(IntPtr font, string text, ref Vector2 position);

		/// <summary>
		///	Draws Texture2D.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void DrawTexture2D(IntPtr texturePtr, ref RectangleF rect, ref RectangleF uvs);

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
