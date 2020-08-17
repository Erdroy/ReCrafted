// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Content;
using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering 
{
    /// <summary>
    ///     Model asset class.
    /// </summary>
    public sealed partial class Model : BinaryAsset
    {

        /// <summary>
        ///     Gets all of the bound materials from this model.
        /// </summary>
        public Material[] Materials
        {
            get
            {
                Get_InternalMaterials(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMaterials(System.IntPtr nativePtr, out Material[] data);
    }
}