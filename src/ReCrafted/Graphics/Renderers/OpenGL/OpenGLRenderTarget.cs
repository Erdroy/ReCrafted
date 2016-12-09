// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Diagnostics.CodeAnalysis;
using SharpDX;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    [SuppressMessage("ReSharper", "InconsistentNaming")]
    internal class OpenGLRenderTarget : RenderTarget
    {
        public override void Resize(int width, int height)
        {
            throw new System.NotImplementedException();
        }

        public override void Clear(Color color)
        {
            throw new System.NotImplementedException();
        }

        public override void Dispose()
        {
            throw new System.NotImplementedException();
        }
    }
}
