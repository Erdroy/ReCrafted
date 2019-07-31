// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Content;
using ReCrafted.API.Rendering;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Voxels 
{
    public sealed class VoxelMaterial : JsonAsset
    {
        
        public Texture GetTexture(VoxelMaterialType type)
        {
            return InternalGetTexture(NativePtr, type);
        }

        public string Name
        {
            get
            {
                Get_InternalName(NativePtr, out var data);
                return data;
            }
        }

        public ushort Material
        {
            get
            {
                Get_InternalMaterial(NativePtr, out var data);
                return data;
            }
        }

        public sbyte Hardness
        {
            get
            {
                Get_InternalHardness(NativePtr, out var data);
                return data;
            }
        }

        public bool HasNormalTextures
        {
            get
            {
                Get_InternalHasNormalTextures(NativePtr, out var data);
                return data;
            }
        }

        public bool HasFarTextures
        {
            get
            {
                Get_InternalHasFarTextures(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Texture InternalGetTexture(System.IntPtr nativePtr, VoxelMaterialType type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalName(System.IntPtr nativePtr, out string data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMaterial(System.IntPtr nativePtr, out ushort data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalHardness(System.IntPtr nativePtr, out sbyte data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalHasNormalTextures(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalHasFarTextures(System.IntPtr nativePtr, out bool data);
    }
}