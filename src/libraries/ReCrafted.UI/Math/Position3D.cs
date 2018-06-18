// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.UI.Math
{
    public struct Position3D
    {
        public Position3D(float x, float y, float z) : this()
        {
            X = x;
            Y = y;
            Z = z;
        }

        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }
    }
}
