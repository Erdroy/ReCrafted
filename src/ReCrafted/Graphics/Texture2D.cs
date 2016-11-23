using System;

namespace ReCrafted.Graphics
{
    public class Texture2D : IDisposable
    {
        protected Texture2D() { }

        public virtual void Dispose() { }

        public static Texture2D New(int width, int height)
        {
            return null;
        }

        public static Texture2D FromFile(string file)
        {
            return null;
        }

        public string Name { get; set; }
    }
}