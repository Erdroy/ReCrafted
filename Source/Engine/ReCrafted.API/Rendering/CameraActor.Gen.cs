// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Core.Actors;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering 
{
    /// <summary>
    ///     CameraActor actor class.
    /// </summary>
    public sealed class CameraActor : ActorBase
    {

        /// <summary>
        ///     Gets the camera contained by this actor. See also: <see cref="ReCrafted.API.Rendering.Camera"/>.
        /// </summary>
        public Camera Camera
        {
            get
            {
                Get_InternalCamera(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalCamera(System.IntPtr nativePtr, out Camera data);
    }
}