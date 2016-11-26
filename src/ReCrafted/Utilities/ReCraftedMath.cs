
namespace ReCrafted.Utilities
{
    /// <summary>
    /// Math class.
    /// </summary>
    public static class ReCraftedMath
    {
        /// <summary>
        /// Round input value up to nearest power of 2.
        /// </summary>
        /// <param name="value">The input value.</param>
        /// <returns>The rounded number.</returns>
        public static int RoundUpToPowerOf2(int value)
        {
            var v = value;

            v--;
            v |= v >> 1;
            v |= v >> 2;
            v |= v >> 4;
            v |= v >> 8;
            v |= v >> 16;
            v++;

            return v;
        }
    }
}