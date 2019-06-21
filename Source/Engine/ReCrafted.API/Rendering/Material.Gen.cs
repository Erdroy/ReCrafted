// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering 
{
    /// <summary>
    ///     Material class.
    /// </summary>
    public sealed partial class Material : Object
    {

        /// <summary>
        ///     Gets the shader that this material uses.
        /// </summary>
        public Shader Shader
        {
            get
            {
                Get_InternalShader(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalShader(System.IntPtr nativePtr, out Shader data);
    }
}