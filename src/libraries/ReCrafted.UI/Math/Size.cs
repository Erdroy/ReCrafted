// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.UI.Math
{
    public struct Size
    {
        public Size(float width, float height) : this()
        {
            Width = width;
            Height = height;
        }

        public float Width { get; set; }
        public float Height { get; set; }
    }
}
