// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using ObjLoader.Loader.Loaders;
using ReCrafted.Graphics;
using SharpDX;

namespace ModelConverter
{
    internal class Program
    {
        private static void Main(string[] args)
        {
            var objLoaderFactory = new ObjLoaderFactory();
            var objLoader = objLoaderFactory.Create(new MaterialNullStreamProvider());

            using (var fileStream = new FileStream("obj.obj", FileMode.Open))
            {
                var result = objLoader.Load(fileStream);

                var verts = new List<Vector3>();
                var normals = new List<Vector3>();
                var colors = new List<Color>();
                var uvs = new List<Vector2>();
                var indices = new List<uint>();

                // load indices
                foreach (var group in result.Groups)
                {
                    foreach (var face in group.Faces)
                    {
                        for (var i = 0; i < face.Count; i++)
                        {
                            indices.Add((uint)face[i].VertexIndex);
                        }
                    }
                }

                // load verts
                foreach (var v in result.Vertices)
                {
                    verts.Add(new Vector3(v.X, v.Y, v.Z));
                }

                // TODO: Implement rest of the mesh inputs

                var meshData = new MeshData
                {
                    Positions = verts.ToArray(),
                    Indices = indices.ToArray()
                };

                File.WriteAllText("obj.json", JsonConvert.SerializeObject(meshData, Formatting.Indented));
            }
        }
    }
}