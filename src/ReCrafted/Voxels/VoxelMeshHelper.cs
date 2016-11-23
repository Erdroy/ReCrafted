using System.Collections.Generic;
using SharpDX;

namespace ReCrafted.Voxels
{
    public static class VoxelMeshHelper
    {

        public static void SetupFace(Vector3 origin, Vector3 up, Vector3 right, bool isReversed, ICollection<Vector3> vertices, ICollection<Vector2> uVs, ICollection<uint> triangles)
        {
            var index = (uint)vertices.Count;

            vertices.Add(origin);
            vertices.Add(origin + up);
            vertices.Add(origin + up + right);
            vertices.Add(origin + right);

            uVs.Add(new Vector2(0, 0));
            uVs.Add(new Vector2(0, 1));
            uVs.Add(new Vector2(1, 1));
            uVs.Add(new Vector2(1, 0));

            if (isReversed)
            {
                triangles.Add(index + 0);
                triangles.Add(index + 1);
                triangles.Add(index + 2);
                triangles.Add(index + 2);
                triangles.Add(index + 3);
                triangles.Add(index + 0);
            }
            else
            {
                triangles.Add(index + 1);
                triangles.Add(index + 0);
                triangles.Add(index + 2);
                triangles.Add(index + 3);
                triangles.Add(index + 2);
                triangles.Add(index + 0);
            }
        }
    }
}