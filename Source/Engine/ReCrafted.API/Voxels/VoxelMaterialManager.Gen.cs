// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Content;
using ReCrafted.API.Rendering;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Voxels 
{
    public static class VoxelMaterialManager
    {
        
        /// <summary>
        ///     Adds material to the registry.
        /// </summary>
        public static void AddMaterial(VoxelMaterial material)
        {
            InternalAddMaterial(material? material.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Sets the default material which is being applied, when
        ///     some voxel uses material id that does not exist.
        /// </summary>
        public static void SetDefaultMaterial(VoxelMaterial material)
        {
            InternalSetDefaultMaterial(material? material.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Gets material by it's id.
        /// </summary>
        public static VoxelMaterial GetMaterial(ushort id)
        {
            return InternalGetMaterial(id);
        }

        /// <summary>
        ///     The default material, that is being used to render procedural meshes of the generated terrains.
        /// </summary>
        public static Material MainMaterial
        {
            get
            {
                Get_InternalMainMaterial(out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalAddMaterial(System.IntPtr material);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetDefaultMaterial(System.IntPtr material);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern VoxelMaterial InternalGetMaterial(ushort id);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalMainMaterial(out Material data);
    }
}