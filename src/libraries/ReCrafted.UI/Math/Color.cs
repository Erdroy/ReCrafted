// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.UI.Math
{
    public struct Color
    {
        public Color(float r, float g, float b, float a) : this()
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public float R { get; set; }
        public float G { get; set; }
        public float B { get; set; }
        public float A { get; set; }

        public static Color White => new Color(1.0f, 1.0f, 1.0f, 1.0f);
        public static Color Black => new Color(0.0f, 0.0f, 0.0f, 1.0f);
        public static Color Red => new Color(1.0f, 0.0f, 0.0f, 1.0f);
        public static Color Green => new Color(0.0f, 1.0f, 0.0f, 1.0f);
        public static Color Blue => new Color(0.0f, 0.0f, 1.0f, 1.0f);
        public static Color DarkGray => new Color(0.35f, 0.35f, 0.35f, 1.0f);
    }
}
