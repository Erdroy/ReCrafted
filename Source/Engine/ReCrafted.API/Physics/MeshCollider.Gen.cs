// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Rendering;
using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Physics 
{
    /// <summary>
    ///     MeshCollider class.
    /// </summary>
    /// <remarks>Before attaching to an actor, use SetMesh to fully initialize this class.</remarks>
    /// <remarks>MeshCollider is only valid for static and dynamic kinematic RigidBodies.</remarks>
    public sealed class MeshCollider : Collider
    {
        
        /// <summary>
        ///     Makes this collider to match given mesh.
        /// </summary>
        /// <remarks>To properly use mesh, please make sure that it is not applied or is applied with 'releaseMeshData' flag in ApplyChanges.</remarks>
        public void SetMesh(Mesh mesh)
        {
            InternalSetMesh(NativePtr, mesh? mesh.NativePtr : System.IntPtr.Zero);
        }
        
        /// <summary>
        ///     Makes this collider to match given raw/procedural mesh data.
        /// </summary>
        public void SetMesh(Vector3[] vertices, uint[] indices)
        {
            InternalSetMesh(NativePtr, vertices, indices);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetMesh(System.IntPtr nativePtr, System.IntPtr mesh);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetMesh(System.IntPtr nativePtr, Vector3[] vertices, uint[] indices);
    }
}