// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Content;
using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering 
{
    /// <summary>
    ///     Material class.
    /// </summary>
    public sealed partial class Material : JsonAsset
    {

        /// <summary>
        ///     Gets or sets the material name.
        /// </summary>
        public string MaterialName
        {
            get
            {
                Get_InternalMaterialName(NativePtr, out var data);
                return data;
            }
            set => Set_InternalMaterialName(NativePtr, ref value);
        }

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
        private static extern void Get_InternalMaterialName(System.IntPtr nativePtr, out string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Set_InternalMaterialName(System.IntPtr nativePtr, ref string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalShader(System.IntPtr nativePtr, out Shader data);
    }
}