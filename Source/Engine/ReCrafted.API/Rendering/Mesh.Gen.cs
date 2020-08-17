// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// WARNING: Auto-generated file. All changes will be lost when the API code will be regenerated!

using ReCrafted.API.Mathematics;
using System.Runtime.CompilerServices;

namespace ReCrafted.API.Rendering 
{
    /// <summary>
    ///     Mesh class.
    /// </summary>
    public sealed class Mesh : Object
    {
        
        /// <summary>
        ///     Set vertices for this mesh.
        /// </summary>
        /// <param name="vertices">The vertices array.</param>
        /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
        public void SetVertices(Vector3[] vertices)
        {
            InternalSetVertices(NativePtr, vertices);
        }
        
        /// <summary>
        ///     Set uvs for this mesh.
        /// </summary>
        /// <param name="uvs">uvs pointer.</param>
        /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
        /// <remarks>The count of uvs must be the same as the vertex count!</remarks>
        public void SetUVs(Vector2[] uvs)
        {
            InternalSetUVs(NativePtr, uvs);
        }
        
        /// <summary>
        ///     Set normals for this mesh.
        /// </summary>
        /// <param name="normals">normals pointer.</param>
        /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
        public void SetNormals(Vector3[] normals)
        {
            InternalSetNormals(NativePtr, normals);
        }
        
        /// <summary>
        ///     Set colors for this mesh.
        /// </summary>
        /// <param name="colors">colors pointer.</param>
        /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
        public void SetColors(Vector4[] colors)
        {
            InternalSetColors(NativePtr, colors);
        }
        
        /// <summary>
        ///     Set indices for this mesh.
        /// </summary>
        /// <param name="indices">The array of indices.</param>
        /// <remarks>To apply changes, you must call 'ApplyChanges' after setting all data you need.</remarks>
        public void SetIndices(uint[] indices)
        {
            InternalSetIndices(NativePtr, indices);
        }
        
        /// <summary>
        ///     Applies all changes.
        /// </summary>
        /// <param name="releaseMeshData">
        ///     When true, this function will release the mesh data (vertices, colors, indices etc.)
        ///     and all GetVertices/GetNormals will become not available.
        /// </param>
        /// <param name="autoUpload">Should this mesh be uploaded after all changes are applied?</param>
        /// <param name="asyncUpload">Should this mesh be uploaded using async function (i.e. dispatched to the render thread)?</param>
        /// <remarks>When mesh is being uploaded using async, it will be available in the NEXT FRAME for rendering.</remarks>
        public void ApplyChanges(bool releaseMeshData = true, bool autoUpload = true, bool asyncUpload = true)
        {
            InternalApplyChanges(NativePtr, releaseMeshData, autoUpload, asyncUpload);
        }
        
        /// <summary>
        ///     Upload buffers to the GPU.
        /// </summary>
        /// <param name="asyncUpload">Should mesh be uploaded using async function (i.e. dispatched to the render thread)?</param>
        /// <remarks>When mesh is being uploaded using async, it will be available in the NEXT FRAME for rendering.</remarks>
        public void Upload(bool asyncUpload = true)
        {
            InternalUpload(NativePtr, asyncUpload);
        }
        
        /// <summary>
        ///     Creates new mesh.
        /// </summary>
        /// <returns>The newly created mesh pointer.</returns>
        public static Mesh CreateMesh()
        {
            return InternalCreateMesh();
        }

        /// <summary>
        ///     Gets vertices array from this mesh.
        /// </summary>
        /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
        public Vector3[] Vertices
        {
            get
            {
                Get_InternalVertices(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets uvs array from this mesh.
        /// </summary>
        /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
        public Vector2[] UVs
        {
            get
            {
                Get_InternalUVs(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets normals array from this mesh.
        /// </summary>
        /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
        public Vector3[] Normals
        {
            get
            {
                Get_InternalNormals(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets colors array from this mesh.
        /// </summary>
        /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
        public Vector4[] Colors
        {
            get
            {
                Get_InternalColors(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Gets indices from this mesh.
        /// </summary>
        /// <remarks>Available only before calling ApplyChanges with releaseMeshData parameter as true.</remarks>
        public uint[] Indices
        {
            get
            {
                Get_InternalIndices(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Returns the amount of vertices (and of course the count of uvs, colors and normals, as it is the same).
        /// </summary>
        public int VertexCount
        {
            get
            {
                Get_InternalVertexCount(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Returns the amount of indices.
        /// </summary>
        public int IndexCount
        {
            get
            {
                Get_InternalIndexCount(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Indicates the upload state of this mesh.
        /// </summary>
        /// <returns>True when all changes are uploaded to the GPU.</returns>
        public bool IsUploaded
        {
            get
            {
                Get_InternalIsUploaded(NativePtr, out var data);
                return data;
            }
        }

        /// <summary>
        ///     Can the mesh be uploaded?
        /// </summary>
        /// <returns>True when it can be.</returns>
        public bool CanUpload
        {
            get
            {
                Get_InternalCanUpload(NativePtr, out var data);
                return data;
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetVertices(System.IntPtr nativePtr, Vector3[] vertices);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetUVs(System.IntPtr nativePtr, Vector2[] uvs);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetNormals(System.IntPtr nativePtr, Vector3[] normals);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetColors(System.IntPtr nativePtr, Vector4[] colors);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalSetIndices(System.IntPtr nativePtr, uint[] indices);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalApplyChanges(System.IntPtr nativePtr, bool releaseMeshData, bool autoUpload, bool asyncUpload);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void InternalUpload(System.IntPtr nativePtr, bool asyncUpload);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Mesh InternalCreateMesh();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalVertices(System.IntPtr nativePtr, out Vector3[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalUVs(System.IntPtr nativePtr, out Vector2[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalNormals(System.IntPtr nativePtr, out Vector3[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalColors(System.IntPtr nativePtr, out Vector4[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIndices(System.IntPtr nativePtr, out uint[] data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalVertexCount(System.IntPtr nativePtr, out int data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIndexCount(System.IntPtr nativePtr, out int data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalIsUploaded(System.IntPtr nativePtr, out bool data);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Get_InternalCanUpload(System.IntPtr nativePtr, out bool data);
    }
}