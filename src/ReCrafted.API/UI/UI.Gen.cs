// ReCrafted © 2016-2017 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 12/27/2017 16:28:59 Source: 'UI.API.cpp' Target: 'UI/UI.Gen.cs'

using ReCrafted.API.Mathematics;
using ReCrafted.API.Graphics;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.UI 
{
	/// <summary>
	///	UI internal class.
	/// </summary>
	internal static partial class UIInternal 
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
		///	Sets view rect.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void UpdateViewRect(ref RectangleF viewRect, bool use);

		/// <summary>
		///	Draws Texture2D.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void DrawTexture2D(IntPtr texturePtr, ref RectangleF rect, ref RectangleF uvs);

		/// <summary>
		///	The current drawing color.
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

		/// <summary>
		///	Depth of current drawing ui.
		/// </summary>
		internal static float Depth 
		{
			get
			{
				return Internal_Depth_Get();
			}
			set
			{
				Internal_Depth_Set(value);
			}
		}

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Color_Set(Color value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Color Internal_Color_Get();
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern void Internal_Depth_Set(float value);
		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern float Internal_Depth_Get();
	}
}
