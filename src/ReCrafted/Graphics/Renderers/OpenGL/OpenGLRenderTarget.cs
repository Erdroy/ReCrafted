// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using SharpDX;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    /// <summary>
    /// OpenGLRenderTarget class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    public sealed class OpenGLRenderTarget : RenderTarget
    {
        /// <summary>
        /// OpenGLRenderTarget constructor.
        /// </summary>
        public OpenGLRenderTarget(int width, int height, TextureFormat format, bool uav)
        {
        }

        /// <summary>
        /// Resize the render target.
        /// </summary>
        public override void Resize(int width, int height)
        {
        }

        /// <summary>
        /// Clear the render target using given color.
        /// </summary>
        /// <param name="color">The color.</param>
        public override void Clear(Color color)
        {
        }

        /// <summary>
        /// Dispose the RenderTarget.
        /// </summary>
        public override void Dispose()
        {
        }

        // private
        private void Internal_Create(int width, int height, bool uav)
        {
        }
    }
}
