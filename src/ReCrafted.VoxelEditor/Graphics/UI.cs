// ReCrafted © 2016-2017 Always Too Late

using System;
using ReCrafted.VoxelEditor.Rendering;

namespace ReCrafted.VoxelEditor.Graphics
{
    /// <summary>
    /// UI class. Provides basic user interface rendering.
    /// </summary>
    public class UI : IDisposable
    {
        private Render2D _render2D;

        public void Init()
        {
            _render2D = new Render2D();
            _render2D.Init();
        }

        public void Draw()
        {
            _render2D.Draw();
        }

        public void Dispose()
        {
            _render2D.Dispose();
        }
    }
}
