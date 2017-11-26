// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.VoxelEditor.Rendering;

namespace ReCrafted.VoxelEditor.Graphics
{
    public static class Display
    {
        public static void Update()
        {
            var size = Renderer.Current.RenderForm.ClientSize;
            Width = size.Width;
            Height = size.Height;
        }

        public static float Width { get; internal set; }
        public static float Height { get; internal set; }
    }
}
