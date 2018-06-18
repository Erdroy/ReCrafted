// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.UI.Math
{
    public struct Rotation3D
    {
        public Rotation3D(float x, float y, float z, float w) : this()
        {
            X = x;
            Y = y;
            Z = z;
            W = w;
        }

        public float X { get; set; }
        public float Y { get; set; }
        public float Z { get; set; }
        public float W { get; set; }
    }
}
