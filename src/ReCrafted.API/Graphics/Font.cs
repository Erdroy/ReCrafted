// ReCrafted © 2016-2017 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.Graphics
{
    /// <summary>
    ///	Font class.
    /// </summary>
    public partial class Font
    {
        /// <summary>
        /// Measures size of given string.
        /// </summary>
        public Vector2 MeasureString(string text)
        {
            Vector2 size;
            Internal_MeasureString(NativePtr, text, out size);
            return size;
        }
    }
}
