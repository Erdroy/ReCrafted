// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.API.Mathematics;

namespace ReCrafted.API.UI
{
    /// <summary>
    /// Test methods for ui animations.
    /// </summary>
    public static class UIAnimation
    {
        /// <summary>
        /// Spring function for Vector2.
        /// </summary>
        /// <param name="x">Value.</param>
        /// <param name="v">Velocity.</param>
        /// <param name="xt">Target value.</param>
        /// <param name="h">Time step.</param>
        /// <param name="zeta">Damping ratio.</param>
        /// <param name="omega">Angular frequency.</param>
        public static void SpringVector2(ref Vector2 x, ref Vector2 v, Vector2 xt, float h, float zeta = 0.23f, float omega = 8.0f * MathUtil.Pi)
        {
            float x1 = x.X;
            float x2 = x.Y;
            float v1 = v.X;
            float v2 = v.Y;

            Spring(ref x1, ref v1, xt.X, h, zeta, omega);
            Spring(ref x2, ref v2, xt.Y, h, zeta, omega);

            x.X = x1;
            x.Y = x2;
            v.X = v1;
            v.Y = v2;
        }

        /// <summary>
        /// Spring function.
        /// </summary>
        /// <param name="x">Value.</param>
        /// <param name="v">Velocity.</param>
        /// <param name="xt">Target value.</param>
        /// <param name="h">Time step.</param>
        /// <param name="zeta">Damping ratio.</param>
        /// <param name="omega">Angular frequency.</param>
        public static void Spring(ref float x, ref float v, float xt, float h, float zeta = 0.23f, float omega = 8.0f * MathUtil.Pi)
        {
            float f = 1.0f + 2.0f * h * zeta * omega;
            float oo = omega * omega;
            float hoo = h * oo;
            float hhoo = h * hoo;
            float detInv = 1.0f / (f + hhoo);
            float detX = f * x + h * v + hhoo * xt;
            float detV = v + hoo * (xt - x);
            x = detX * detInv;
            v = detV * detInv;
        }
    }
}
