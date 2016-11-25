using System;
using System.Drawing;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Utilities;

namespace ReCrafted.Graphics
{
    public abstract class Texture2D : IDisposable
    {
        protected Texture2D() { }

        protected abstract void Load(Bitmap bitmap);

        public abstract void Apply(int slot);

        public abstract void Dispose();
        
        public static Texture2D New(int width, int height)
        {
            throw new ReCraftedException("Not implemented!"); // TODO: Implement Texture2D::New
        }

        public static Texture2D FromFile(string file)
        {
            if (Renderer.RendererApi == RendererApi.D3D11)
            {
                var d3D11Texture = new D3D11Texture2D();
                d3D11Texture.Load(new Bitmap(file));
                return d3D11Texture;
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }

        public string Name { get; set; }
    }
}