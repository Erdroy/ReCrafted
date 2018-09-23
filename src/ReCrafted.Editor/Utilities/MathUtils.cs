// ReCrafted Editor (c) 2016-2018 Always Too Late

namespace ReCrafted.Editor.Utilities
{
    public class MathUtils
    {
        public static bool IsPowerOf2(int x)
        {
            // source: https://stackoverflow.com/questions/600293/how-to-check-if-a-number-is-a-power-of-2
            return x != 0 && (x & (x - 1)) == 0;
        }
    }
}
