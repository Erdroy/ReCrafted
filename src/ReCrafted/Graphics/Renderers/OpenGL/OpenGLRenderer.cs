// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Diagnostics.CodeAnalysis;
using SharpDX;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    [SuppressMessage("ReSharper", "InconsistentNaming")]
    internal class OpenGLRenderer : Renderer
    {
        protected override void Init()
        {
            throw new System.NotImplementedException();
        }

        public override void Tick()
        {
            throw new System.NotImplementedException();
        }

        public override void Draw()
        {
            throw new System.NotImplementedException();
        }

        public override void Resize(int width, int height)
        {
            throw new System.NotImplementedException();
        }

        public override void SetRasterizerState(RasterizerStates state)
        {
            throw new System.NotImplementedException();
        }

        public override void SetDepthTestState(bool enabled)
        {
            throw new System.NotImplementedException();
        }

        public override void SetRenderTargets(params RenderTarget[] renderTargets)
        {
            throw new System.NotImplementedException();
        }

        public override void SetFinalRenderTarget(bool useDepthTest)
        {
            throw new System.NotImplementedException();
        }

        public override void Blit(RenderTarget renderTarget)
        {
            throw new System.NotImplementedException();
        }

        public override void Dispatch(int x, int y, int z)
        {
            throw new System.NotImplementedException();
        }

        public override RenderTarget[] GetRenderTargets()
        {
            throw new System.NotImplementedException();
        }

        public override void DrawBoundingBox(BoundingBox bounds)
        {
            throw new System.NotImplementedException();
        }

        public override void Dispose()
        {
            throw new System.NotImplementedException();
        }
    }
}