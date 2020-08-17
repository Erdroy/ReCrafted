// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
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
        ///     Sets texture array to a given slot.
        /// </summary>
        /// <param name="slot">The texture array slot, starts at 0.</param>
        /// <param name="textureArray">The texture array.</param>
        public void SetTextureArray(int slot, Texture[] textureArray)
        {
            InternalSetTextureArray(NativePtr, slot, textureArray);
        }
        
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
        private static extern void InternalSetTextureArray(System.IntPtr nativePtr, int slot, Texture[] textureArray);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetTexture(System.IntPtr nativePtr, int slot, System.IntPtr texture);
    }
}