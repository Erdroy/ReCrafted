// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;

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

        /// <summary>
        /// Round up to nearest multiple of 
        /// </summary>
        /// <param name="value"></param>
        /// <param name="multiple"></param>
        /// <returns></returns>
        public static float NearestRound(float value, float multiple)
        {
            if (!(multiple < 1))
                return (float) Math.Round(value/multiple, MidpointRounding.AwayFromZero)*multiple;

            var i = (float)Math.Floor(value);
            var x2 = i;
            while ((x2 += multiple) < value)
            {
            }
            var x1 = x2 - multiple;
            return (Math.Abs(value - x1) < Math.Abs(value - x2)) ? x1 : x2;
        }
    }
}