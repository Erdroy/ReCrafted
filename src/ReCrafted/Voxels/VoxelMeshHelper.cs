// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System.Collections.Generic;
using SharpDX;

namespace ReCrafted.Voxels
{
    public static class VoxelMeshHelper
    {
        public static float CalculateAmbient(bool sideA, bool sideB, bool corner)
        {
            if (sideA && sideB)
                return 1.0f;

            return (sideA ? 0.5f : 0) + (sideB ? 0.5f : 0) + (corner ? 0.5f : 0);
        }

        public static void SetupFace(Vector3 origin, Vector3 up, Vector3 right, bool isReversed, bool invertedQuad, 
            ICollection<Vector3> vertices,
            ICollection<Vector2> uVs, 
            ICollection<uint> triangles, 
            ICollection<Vector3> normals)
        {
            var index = (uint)vertices.Count;


            var a = origin;
            var b = origin + up;
            var c = origin + up + right;
            var d = origin + right;
            
            vertices.Add(a);
            vertices.Add(b);
            vertices.Add(c);
            vertices.Add(d);

            // calculate normals
            var dir = new Plane(a, b, c).Normal;

            if (isReversed)
                dir = new Plane(c, b, a).Normal;

            var norm = Vector3.Normalize(dir);

            normals.Add(norm);
            normals.Add(norm);
            normals.Add(norm);
            normals.Add(norm);

            uVs.Add(new Vector2(0.0f, 0.0f));
            uVs.Add(new Vector2(0.0f, 1.0f));
            uVs.Add(new Vector2(1.0f, 1.0f));
            uVs.Add(new Vector2(1.0f, 0.0f));

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