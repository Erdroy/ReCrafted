// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Content;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering 
{
    /// <summary>
    ///     Texture asset class.
    /// </summary>
    public sealed class Texture : BinaryAsset
    {
        
        /// <summary>
        ///     Initializes this texture from given file.
        /// </summary>
        /// <param name="fileName">The file which will be loaded as texture.</param>
        /// <remarks>Only PNG RGBA8 format is supported! Requires Apply call.</remarks>
        public void InitializeFromFile(string fileName)
        {
            InternalInitializeFromFile(NativePtr, fileName);
        }
        
        /// <summary>
        ///     Get pixel at given position.
        /// </summary>
        /// <param name="x">The start X coord.</param>
        /// <param name="y">The start Y coord.</param>
        /// <remarks>Available only before Apply call, when format is RGBA8 and has been initialized(Texture::Initialize(...)).</remarks>
        public uint GetPixel(int x, int y)
        {
            return InternalGetPixel(NativePtr, x, y);
        }
        
        /// <summary>
        ///     Sets pixel at given position.
        /// </summary>
        /// <param name="x">The start X coord.</param>
        /// <param name="y">The start Y coord.</param>
        /// <param name="pixel">Pixel in HEX format.</param>
        /// <remarks>Available only before Apply call, when format is RGBA8 and has been initialized(Texture::Initialize(...)).</remarks>
        public void SetPixel(int x, int y, uint pixel)
        {
            InternalSetPixel(NativePtr, x, y, pixel);
        }
        
        /// <summary>
        ///     Sets pixel at given position.
        /// </summary>
        /// <param name="x">The start X coord.</param>
        /// <param name="y">The start Y coord.</param>
        /// <param name="r">R channel value.</param>
        /// <param name="g">G channel value.</param>
        /// <param name="b">B channel value.</param>
        /// <param name="a">A channel value.</param>
        /// <remarks>Available only before Apply call, when format is RGBA8 and has been initialized(Texture::Initialize(...)).</remarks>
        public void SetPixel(int x, int y, byte r, byte g, byte b, byte a)
        {
            InternalSetPixel(NativePtr, x, y, r, g, b, a);
        }
        
        /// <summary>
        ///     Resizes the texture to given size.
        /// </summary>
        /// <param name="width">The new texture width.</param>
        /// <param name="height">The new texture height.</param>
        /// <remarks>Only texture created as Virtual Asset with Initialize function can be resized.</remarks>
        /// <remarks>Mip-maps are not supported.</remarks>
        public void Resize(ushort width, ushort height)
        {
            InternalResize(NativePtr, width, height);
        }

        /// <summary>
        ///     Gets the width of this texture.
        /// </summary>
        public uint Width
        {
            get
            {
                Get_InternalWidth(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the height of this texture.
        /// </summary>
        public uint Height
        {
            get
            {
                Get_InternalHeight(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets the texture 3D state. True, when this texture is 3D texture false when 2D.
        /// </summary>
        public bool Is3D
        {
            get
            {
                Get_InternalIs3D(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalInitializeFromFile(System.IntPtr nativePtr, string fileName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern uint InternalGetPixel(System.IntPtr nativePtr, int x, int y);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetPixel(System.IntPtr nativePtr, int x, int y, uint pixel);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetPixel(System.IntPtr nativePtr, int x, int y, byte r, byte g, byte b, byte a);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalResize(System.IntPtr nativePtr, ushort width, ushort height);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalWidth(System.IntPtr nativePtr, out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalHeight(System.IntPtr nativePtr, out uint data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIs3D(System.IntPtr nativePtr, out bool data);
    }
}