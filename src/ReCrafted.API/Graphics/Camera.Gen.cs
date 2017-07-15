// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 07/15/2017 19:27:36 Source: 'Object.API.cpp' Target: 'Graphics/Camera.Gen.cs'

using System.Runtime.CompilerServices;

namespace ReCrafted.API.Graphics 
{
	/// <summary>
	///	Camera class.
	/// </summary>
	public class Camera : Object
	{
		/// <summary>
		///	Sets the camera as current.
		/// </summary>
		[MethodImpl(MethodImplOptions.InternalCall)]
		public extern void SetAsCurrent();

		/// <summary>
		///	Is game running state.
		/// </summary>
		public static Camera Current => Internal_Current_Get();

		[MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern Camera Internal_Current_Get();
	}
}
