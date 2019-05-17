// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Content;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering 
{
    /// <summary>
    ///     Shader class.
    /// </summary>
    public sealed class Shader : JsonAsset
    {
        
        /// <summary>
        ///     Sets texture at given slot.
        /// </summary>
        /// <param name="slot">The texture slot, starts at 0.</param>
        /// <param name="texture">The texture.</param>
        public void SetTexture(int slot, Texture texture)
        {
            InternalSetTexture(NativePtr, slot, texture? texture.NativePtr : System.IntPtr.Zero);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetTexture(System.IntPtr nativePtr, int slot, System.IntPtr texture);
    }
}