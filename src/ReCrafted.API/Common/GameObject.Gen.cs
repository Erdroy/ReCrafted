// ReCrafted (c) 2016-2018 Always Too Late
// WARNING: Auto-generated file, all changes will be lost when the API code will be regenerated!
// Generated: 10/27/2018 12:42:40 Source: 'GameObject.API.cpp' Target: 'Common/GameObject.Gen.cs'

using ReCrafted.API.Common.Components;
using System;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Common 
{
	public sealed partial class GameObject : Object
	{
        [MethodImpl(MethodImplOptions.InternalCall)]
		internal static extern unsafe TransformComponent* Internal_GetTransform(IntPtr nativePtr);

	}
}
