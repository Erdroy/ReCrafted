// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using SharpDX;

namespace ReCrafted.Graphics.Primitives
{
    /// <summary>
    /// IcoSphere primitive.
    /// original: http://wiki.unity3d.com/index.php/ProceduralPrimitives
    /// </summary>
    public sealed class IcoSphere : Primitive
    {
        private struct TriangleIndices
        {
            public readonly int V1;
            public readonly int V2;
            public readonly int V3;

            public TriangleIndices(int v1, int v2, int v3)
            {
                V1 = v1;
                V2 = v2;
                V3 = v3;
            }
        }

        private readonly int _recursionLevel;
        private readonly float _radius;
        private readonly bool _invert;

        /// <summary>
        /// Initialize IcoSphere
        /// </summary>
        /// <param name="recursionLevel"></param>
        /// <param name="radius"></param>
        /// <param name="invert"></param>
        public IcoSphere(int recursionLevel, float radius, bool invert)
        {
            _recursionLevel = recursionLevel;
            _radius = radius;
            _invert = invert;
        }

        /// <summary>
        /// Create mesh data.
        /// </summary>
        /// <returns>The primitive mesh data.</returns>
        public override MeshData GetMeshData()
        {
			var meshData = new MeshData();

            var vertList = new List<Vector3>();
            var middlePointIndexCache = new Dictionary<long, int>();
			
            // create 12 vertices of a icosahedron
            var t = (1f + (float)Math.Sqrt(5f)) / 2f;

            vertList.Add(Vector3.Normalize(new Vector3(-1f, t, 0f)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(1f, t, 0f)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(-1f, -t, 0f)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(1f, -t, 0f)) * _radius);

            vertList.Add(Vector3.Normalize(new Vector3(0f, -1f, t)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(0f, 1f, t)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(0f, -1f, -t)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(0f, 1f, -t)) * _radius);

            vertList.Add(Vector3.Normalize(new Vector3(t, 0f, -1f)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(t, 0f, 1f)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(-t, 0f, -1f)) * _radius);
            vertList.Add(Vector3.Normalize(new Vector3(-t, 0f, 1f)) * _radius);
			
            // create 20 triangles of the icosahedron
            var faces = new List<TriangleIndices>
            {
                new TriangleIndices(0, 11, 5),
                new TriangleIndices(0, 5, 1),
                new TriangleIndices(0, 1, 7),
                new TriangleIndices(0, 7, 10),
                new TriangleIndices(0, 10, 11),
                new TriangleIndices(1, 5, 9),
                new TriangleIndices(5, 11, 4),
                new TriangleIndices(11, 10, 2),
                new TriangleIndices(10, 7, 6),
                new TriangleIndices(7, 1, 8),
                new TriangleIndices(3, 9, 4),
                new TriangleIndices(3, 4, 2),
                new TriangleIndices(3, 2, 6),
                new TriangleIndices(3, 6, 8),
                new TriangleIndices(3, 8, 9),
                new TriangleIndices(4, 9, 5),
                new TriangleIndices(2, 4, 11),
                new TriangleIndices(6, 2, 10),
                new TriangleIndices(8, 6, 7),
                new TriangleIndices(9, 8, 1)
            };

            // refine triangles
            for (var i = 0; i < _recursionLevel; i++)
            {
                var faces2 = new List<TriangleIndices>();
                foreach (var tri in faces)
                {
                    // replace triangle by 4 triangles
                    var a = GetMiddlePoint(tri.V1, tri.V2, ref vertList, ref middlePointIndexCache, _radius);
                    var b = GetMiddlePoint(tri.V2, tri.V3, ref vertList, ref middlePointIndexCache, _radius);
                    var c = GetMiddlePoint(tri.V3, tri.V1, ref vertList, ref middlePointIndexCache, _radius);

                    faces2.Add(new TriangleIndices(tri.V1, a, c));
                    faces2.Add(new TriangleIndices(tri.V2, b, a));
                    faces2.Add(new TriangleIndices(tri.V3, c, b));
                    faces2.Add(new TriangleIndices(a, b, c));
                }
                faces = faces2;
            }

            meshData.Positions = vertList.ToArray();

            var triList = new List<uint>();
            for (var i = 0; i < faces.Count; i++)
            {
                if (_invert)
                {
                    triList.Add((uint)faces[i].V3);
                    triList.Add((uint)faces[i].V2);
                    triList.Add((uint)faces[i].V1);
                }
                else
                {
                    triList.Add((uint)faces[i].V1);
                    triList.Add((uint)faces[i].V2);
                    triList.Add((uint)faces[i].V3);
                }
            }
            meshData.Indices = triList.ToArray();

            return meshData;
        }

        // return index of point in the middle of p1 and p2
        private static int GetMiddlePoint(int p1, int p2, ref List<Vector3> vertices, ref Dictionary<long, int> cache, float radius)
        {
            // first check if we have it already
            var firstIsSmaller = p1 < p2;
            long smallerIndex = firstIsSmaller ? p1 : p2;
            long greaterIndex = firstIsSmaller ? p2 : p1;
            var key = (smallerIndex << 32) + greaterIndex;

            int ret;
            if (cache.TryGetValue(key, out ret))
            {
                return ret;
            }

            // not in cache, calculate it
            var point1 = vertices[p1];
            var point2 = vertices[p2];
            var middle = new Vector3
            (
                (point1.X + point2.X) / 2f,
                (point1.Y + point2.Y) / 2f,
                (point1.Z + point2.Z) / 2f
            );

            // add vertex makes sure point is on unit sphere
            var i = vertices.Count;
            middle.Normalize();
            vertices.Add(middle * radius);

            // store it, return index
            cache.Add(key, i);

            return i;
        }

    }
}
