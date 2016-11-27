// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using ReCrafted.Graphics;
using ReCrafted.Graphics.Renderers.D3D11;
using SharpDX;

namespace ReCrafted.Voxels
{
    /// <summary>
    /// VoxelCursor class.
    /// </summary>
    public sealed class VoxelCursor : IDisposable
    {
        private Mesh _mesh;
        private Shader _shader;
        
        public void Init()
        {
            _mesh = Mesh.Create();
            _mesh.SetVertices(new []
            {
                new Vector3(0.0f, 0.0f, 0.0f),
                new Vector3(0.0f, 1.0f, 0.0f)
            });

            _mesh.SetColors(new []
            {
                new Color(1.0f, 1.0f, 1.0f, 1.0f),
                new Color(1.0f, 1.0f, 1.0f, 1.0f)
            });

            _mesh.ApplyChanges();

            _mesh.PrimitiveType = PrimitiveType.LineList;

            _shader = Shader.FromFile("color");
        }
        
        public void Draw()
        {
            Renderer.Instance.SetDepthTestState(false);

            var wvp = Matrix.Translation(new Vector3(0.0f, 5.0f, 0.0f)) * Camera.Current.ViewProjectionMatrix;
            wvp.Transpose();

            _shader.SetValue("WVP", wvp);
            _shader.Draw(_mesh);

            Renderer.Instance.SetDepthTestState(true);
        }

        public void Dispose()
        {
            _mesh?.Dispose();
        }
    }
}