﻿// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.
// Original code: https://github.com/sharpdx/SharpDX
// Copyright (c) 2010-2014 SharpDX - Alexandre Mutel
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
// -----------------------------------------------------------------------------
// Original code from SlimMath project. http://code.google.com/p/slimmath/
// Greetings to SlimDX Group. Original code published with the following license:
// -----------------------------------------------------------------------------
/*
* Copyright (c) 2007-2011 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

using System;
using System.Globalization;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace ReCrafted.API.Mathematics
{
    /// <summary>
    /// Represents a color in the form of rgba.
    /// </summary>
    [StructLayout(LayoutKind.Sequential, Pack = 4)]
    public struct Color4 : IEquatable<Color4>, IFormattable
    {
        private const string toStringFormat = "Alpha:{0} Red:{1} Green:{2} Blue:{3}";

        /// <summary>
        /// The Black color (0, 0, 0, 1).
        /// </summary>
        public static readonly Color4 Black = new Color4(0.0f, 0.0f, 0.0f, 1.0f);

        /// <summary>
        /// The White color (1, 1, 1, 1).
        /// </summary>
        public static readonly Color4 White = new Color4(1.0f, 1.0f, 1.0f, 1.0f);

        /// <summary>
        /// The red component of the color.
        /// </summary>
        public float Red;

        /// <summary>
        /// The green component of the color.
        /// </summary>
        public float Green;

        /// <summary>
        /// The blue component of the color.
        /// </summary>
        public float Blue;

        /// <summary>
        /// The alpha component of the color.
        /// </summary>
        public float Alpha;

        /// <summary>
        /// Initializes a new instance of the <see cref="Color4"/> struct.
        /// </summary>
        /// <param name="value">The value that will be assigned to all components.</param>
        public Color4(float value)
        {
            Alpha = Red = Green = Blue = value;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Color4"/> struct.
        /// </summary>
        /// <param name="red">The red component of the color.</param>
        /// <param name="green">The green component of the color.</param>
        /// <param name="blue">The blue component of the color.</param>
        /// <param name="alpha">The alpha component of the color.</param>
        public Color4(float red, float green, float blue, float alpha)
        {
            Red = red;
            Green = green;
            Blue = blue;
            Alpha = alpha;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Color4"/> struct.
        /// </summary>
        /// <param name="value">The red, green, blue, and alpha components of the color.</param>
        public Color4(Vector4 value)
        {
            Red = value.X;
            Green = value.Y;
            Blue = value.Z;
            Alpha = value.W;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Color4"/> struct.
        /// </summary>
        /// <param name="value">The red, green, and blue components of the color.</param>
        /// <param name="alpha">The alpha component of the color.</param>
        public Color4(Vector3 value, float alpha)
        {
            Red = value.X;
            Green = value.Y;
            Blue = value.Z;
            Alpha = alpha;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Color4"/> struct.
        /// </summary>
        /// <param name="rgba">A packed integer containing all four color components in RGBA order.</param>
        public Color4(uint rgba)
        {
            Alpha = ((rgba >> 24) & 255) / 255.0f;
            Blue = ((rgba >> 16) & 255) / 255.0f;
            Green = ((rgba >> 8) & 255) / 255.0f;
            Red = (rgba & 255) / 255.0f;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Color4"/> struct.
        /// </summary>
        /// <param name="rgba">A packed integer containing all four color components in RGBA order.</param>
        public Color4(int rgba)
        {
            Alpha = ((rgba >> 24) & 255) / 255.0f;
            Blue = ((rgba >> 16) & 255) / 255.0f;
            Green = ((rgba >> 8) & 255) / 255.0f;
            Red = (rgba & 255) / 255.0f;
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Color4"/> struct.
        /// </summary>
        /// <param name="values">The values to assign to the red, green, blue, and alpha components of the color. This must be an array with four elements.</param>
        /// <exception cref="ArgumentNullException">Thrown when <paramref name="values"/> is <c>null</c>.</exception>
        /// <exception cref="ArgumentOutOfRangeException">Thrown when <paramref name="values"/> contains more or less than four elements.</exception>
        public Color4(float[] values)
        {
            if (values == null)
                throw new ArgumentNullException("values");
            if (values.Length != 4)
                throw new ArgumentOutOfRangeException("values", "There must be four and only four input values for Color4.");

            Red = values[0];
            Green = values[1];
            Blue = values[2];
            Alpha = values[3];
        }
        
        /// <summary>
        /// Gets or sets the component at the specified index.
        /// </summary>
        /// <value>The value of the red, green, blue, and alpha components, depending on the index.</value>
        /// <param name="index">The index of the component to access. Use 0 for the alpha component, 1 for the red component, 2 for the green component, and 3 for the blue component.</param>
        /// <returns>The value of the component at the specified index.</returns>
        /// <exception cref="System.ArgumentOutOfRangeException">Thrown when the <paramref name="index"/> is out of the range [0, 3].</exception>
        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0: return Red;
                    case 1: return Green;
                    case 2: return Blue;
                    case 3: return Alpha;
                }

                throw new ArgumentOutOfRangeException("index", "Indices for Color4 run from 0 to 3, inclusive.");
            }

            set
            {
                switch (index)
                {
                    case 0: Red = value; break;
                    case 1: Green = value; break;
                    case 2: Blue = value; break;
                    case 3: Alpha = value; break;
                    default: throw new ArgumentOutOfRangeException("index", "Indices for Color4 run from 0 to 3, inclusive.");
                }
            }
        }

        /// <summary>
        /// Converts the color into a packed integer.
        /// </summary>
        /// <returns>A packed integer containing all four color components.</returns>
        public int ToBgra()
        {
            uint a = (uint)(Alpha * 255.0f) & 255;
            uint r = (uint)(Red * 255.0f) & 255;
            uint g = (uint)(Green * 255.0f) & 255;
            uint b = (uint)(Blue * 255.0f) & 255;

            uint value = b;
            value |= g << 8;
            value |= r << 16;
            value |= a << 24;

            return (int)value;
        }

        /// <summary>
        /// Converts the color into a packed integer.
        /// </summary>
        /// <returns>A packed integer containing all four color components.</returns>
        public void ToBgra(out byte r, out byte g, out byte b, out byte a)
        {
            a = (byte)(Alpha * 255.0f);
            r = (byte)(Red * 255.0f);
            g = (byte)(Green * 255.0f);
            b = (byte)(Blue * 255.0f);
        }

        /// <summary>
        /// Converts the color into a packed integer.
        /// </summary>
        /// <returns>A packed integer containing all four color components.</returns>
        public int ToRgba()
        {
            uint a = (uint) (Alpha * 255.0f) & 255;
            uint r = (uint) (Red * 255.0f) & 255;
            uint g = (uint) (Green * 255.0f) & 255;
            uint b = (uint) (Blue * 255.0f) & 255;

            uint value = r;
            value |= g << 8;
            value |= b << 16;
            value |= a << 24;

            return (int)value;
        }

        /// <summary>
        /// Converts the color into a three component vector.
        /// </summary>
        /// <returns>A three component vector containing the red, green, and blue components of the color.</returns>
        public Vector3 ToVector3()
        {
            return new Vector3(Red, Green, Blue);
        }

        /// <summary>
        /// Converts the color into a four component vector.
        /// </summary>
        /// <returns>A four component vector containing all four color components.</returns>
        public Vector4 ToVector4()
        {
            return new Vector4(Red, Green, Blue, Alpha);
        }

        /// <summary>
        /// Creates an array containing the elements of the color.
        /// </summary>
        /// <returns>A four-element array containing the components of the color.</returns>
        public float[] ToArray()
        {
            return new float[] { Red, Green, Blue, Alpha };
        }

        /// <summary>
        /// Adds two colors.
        /// </summary>
        /// <param name="left">The first color to add.</param>
        /// <param name="right">The second color to add.</param>
        /// <param name="result">When the method completes, completes the sum of the two colors.</param>
        public static void Add(ref Color4 left, ref Color4 right, out Color4 result)
        {
            result.Alpha = left.Alpha + right.Alpha;
            result.Red = left.Red + right.Red;
            result.Green = left.Green + right.Green;
            result.Blue = left.Blue + right.Blue;
        }

        /// <summary>
        /// Adds two colors.
        /// </summary>
        /// <param name="left">The first color to add.</param>
        /// <param name="right">The second color to add.</param>
        /// <returns>The sum of the two colors.</returns>
        public static Color4 Add(Color4 left, Color4 right)
        {
            return new Color4(left.Red + right.Red, left.Green + right.Green, left.Blue + right.Blue, left.Alpha + right.Alpha);
        }

        /// <summary>
        /// Subtracts two colors.
        /// </summary>
        /// <param name="left">The first color to subtract.</param>
        /// <param name="right">The second color to subtract.</param>
        /// <param name="result">WHen the method completes, contains the difference of the two colors.</param>
        public static void Subtract(ref Color4 left, ref Color4 right, out Color4 result)
        {
            result.Alpha = left.Alpha - right.Alpha;
            result.Red = left.Red - right.Red;
            result.Green = left.Green - right.Green;
            result.Blue = left.Blue - right.Blue;
        }

        /// <summary>
        /// Subtracts two colors.
        /// </summary>
        /// <param name="left">The first color to subtract.</param>
        /// <param name="right">The second color to subtract</param>
        /// <returns>The difference of the two colors.</returns>
        public static Color4 Subtract(Color4 left, Color4 right)
        {
            return new Color4(left.Red - right.Red, left.Green - right.Green, left.Blue - right.Blue, left.Alpha - right.Alpha);
        }

        /// <summary>
        /// Modulates two colors.
        /// </summary>
        /// <param name="left">The first color to modulate.</param>
        /// <param name="right">The second color to modulate.</param>
        /// <param name="result">When the method completes, contains the modulated color.</param>
        public static void Modulate(ref Color4 left, ref Color4 right, out Color4 result)
        {
            result.Alpha = left.Alpha * right.Alpha;
            result.Red = left.Red * right.Red;
            result.Green = left.Green * right.Green;
            result.Blue = left.Blue * right.Blue;
        }

        /// <summary>
        /// Modulates two colors.
        /// </summary>
        /// <param name="left">The first color to modulate.</param>
        /// <param name="right">The second color to modulate.</param>
        /// <returns>The modulated color.</returns>
        public static Color4 Modulate(Color4 left, Color4 right)
        {
            return new Color4(left.Red * right.Red, left.Green * right.Green, left.Blue * right.Blue, left.Alpha * right.Alpha);
        }

        /// <summary>
        /// Scales a color.
        /// </summary>
        /// <param name="value">The color to scale.</param>
        /// <param name="scale">The amount by which to scale.</param>
        /// <param name="result">When the method completes, contains the scaled color.</param>
        public static void Scale(ref Color4 value, float scale, out Color4 result)
        {
            result.Alpha = value.Alpha * scale;
            result.Red = value.Red * scale;
            result.Green = value.Green * scale;
            result.Blue = value.Blue * scale;
        }

        /// <summary>
        /// Scales a color.
        /// </summary>
        /// <param name="value">The color to scale.</param>
        /// <param name="scale">The amount by which to scale.</param>
        /// <returns>The scaled color.</returns>
        public static Color4 Scale(Color4 value, float scale)
        {
            return new Color4(value.Red * scale, value.Green * scale, value.Blue * scale, value.Alpha * scale);
        }

        /// <summary>
        /// Negates a color.
        /// </summary>
        /// <param name="value">The color to negate.</param>
        /// <param name="result">When the method completes, contains the negated color.</param>
        public static void Negate(ref Color4 value, out Color4 result)
        {
            result.Alpha = 1.0f - value.Alpha;
            result.Red = 1.0f - value.Red;
            result.Green = 1.0f - value.Green;
            result.Blue = 1.0f - value.Blue;
        }

        /// <summary>
        /// Negates a color.
        /// </summary>
        /// <param name="value">The color to negate.</param>
        /// <returns>The negated color.</returns>
        public static Color4 Negate(Color4 value)
        {
            return new Color4(1.0f - value.Red, 1.0f - value.Green, 1.0f - value.Blue, 1.0f - value.Alpha);
        }

        /// <summary>
        /// Restricts a value to be within a specified range.
        /// </summary>
        /// <param name="value">The value to clamp.</param>
        /// <param name="min">The minimum value.</param>
        /// <param name="max">The maximum value.</param>
        /// <param name="result">When the method completes, contains the clamped value.</param>
        public static void Clamp(ref Color4 value, ref Color4 min, ref Color4 max, out Color4 result)
        {
            float alpha = value.Alpha;
            alpha = (alpha > max.Alpha) ? max.Alpha : alpha;
            alpha = (alpha < min.Alpha) ? min.Alpha : alpha;

            float red = value.Red;
            red = (red > max.Red) ? max.Red : red;
            red = (red < min.Red) ? min.Red : red;

            float green = value.Green;
            green = (green > max.Green) ? max.Green : green;
            green = (green < min.Green) ? min.Green : green;

            float blue = value.Blue;
            blue = (blue > max.Blue) ? max.Blue : blue;
            blue = (blue < min.Blue) ? min.Blue : blue;

            result = new Color4(red, green, blue, alpha);
        }

        /// <summary>
        /// Restricts a value to be within a specified range.
        /// </summary>
        /// <param name="value">The value to clamp.</param>
        /// <param name="min">The minimum value.</param>
        /// <param name="max">The maximum value.</param>
        /// <returns>The clamped value.</returns>
        public static Color4 Clamp(Color4 value, Color4 min, Color4 max)
        {
            Color4 result;
            Clamp(ref value, ref min, ref max, out result);
            return result;
        }

        /// <summary>
        /// Performs a linear interpolation between two colors.
        /// </summary>
        /// <param name="start">Start color.</param>
        /// <param name="end">End color.</param>
        /// <param name="amount">Value between 0 and 1 indicating the weight of <paramref name="end"/>.</param>
        /// <param name="result">When the method completes, contains the linear interpolation of the two colors.</param>
        /// <remarks>
        /// Passing <paramref name="amount"/> a value of 0 will cause <paramref name="start"/> to be returned; a value of 1 will cause <paramref name="end"/> to be returned. 
        /// </remarks>
        public static void Lerp(ref Color4 start, ref Color4 end, float amount, out Color4 result)
        {
            result.Red = MathUtil.Lerp(start.Red, end.Red, amount);
            result.Green = MathUtil.Lerp(start.Green, end.Green, amount);
            result.Blue = MathUtil.Lerp(start.Blue, end.Blue, amount);
            result.Alpha = MathUtil.Lerp(start.Alpha, end.Alpha, amount);
        }

        /// <summary>
        /// Performs a linear interpolation between two colors.
        /// </summary>
        /// <param name="start">Start color.</param>
        /// <param name="end">End color.</param>
        /// <param name="amount">Value between 0 and 1 indicating the weight of <paramref name="end"/>.</param>
        /// <returns>The linear interpolation of the two colors.</returns>
        /// <remarks>
        /// Passing <paramref name="amount"/> a value of 0 will cause <paramref name="start"/> to be returned; a value of 1 will cause <paramref name="end"/> to be returned. 
        /// </remarks>
        public static Color4 Lerp(Color4 start, Color4 end, float amount)
        {
            Color4 result;
            Lerp(ref start, ref end, amount, out result);
            return result;
        }

        /// <summary>
        /// Performs a cubic interpolation between two colors.
        /// </summary>
        /// <param name="start">Start color.</param>
        /// <param name="end">End color.</param>
        /// <param name="amount">Value between 0 and 1 indicating the weight of <paramref name="end"/>.</param>
        /// <param name="result">When the method completes, contains the cubic interpolation of the two colors.</param>
        public static void SmoothStep(ref Color4 start, ref Color4 end, float amount, out Color4 result)
        {
            amount = MathUtil.SmoothStep(amount);
            Lerp(ref start, ref end, amount, out result);
        }

        /// <summary>
        /// Performs a cubic interpolation between two colors.
        /// </summary>
        /// <param name="start">Start color.</param>
        /// <param name="end">End color.</param>
        /// <param name="amount">Value between 0 and 1 indicating the weight of <paramref name="end"/>.</param>
        /// <returns>The cubic interpolation of the two colors.</returns>
        public static Color4 SmoothStep(Color4 start, Color4 end, float amount)
        {
            Color4 result;
            SmoothStep(ref start, ref end, amount, out result);
            return result;
        }

        /// <summary>
        /// Returns a color containing the smallest components of the specified colors.
        /// </summary>
        /// <param name="left">The first source color.</param>
        /// <param name="right">The second source color.</param>
        /// <param name="result">When the method completes, contains an new color composed of the largest components of the source colors.</param>
        public static void Max(ref Color4 left, ref Color4 right, out Color4 result)
        {
            result.Alpha = (left.Alpha > right.Alpha) ? left.Alpha : right.Alpha;
            result.Red = (left.Red > right.Red) ? left.Red : right.Red;
            result.Green = (left.Green > right.Green) ? left.Green : right.Green;
            result.Blue = (left.Blue > right.Blue) ? left.Blue : right.Blue;
        }

        /// <summary>
        /// Returns a color containing the largest components of the specified colors.
        /// </summary>
        /// <param name="left">The first source color.</param>
        /// <param name="right">The second source color.</param>
        /// <returns>A color containing the largest components of the source colors.</returns>
        public static Color4 Max(Color4 left, Color4 right)
        {
            Color4 result;
            Max(ref left, ref right, out result);
            return result;
        }

        /// <summary>
        /// Returns a color containing the smallest components of the specified colors.
        /// </summary>
        /// <param name="left">The first source color.</param>
        /// <param name="right">The second source color.</param>
        /// <param name="result">When the method completes, contains an new color composed of the smallest components of the source colors.</param>
        public static void Min(ref Color4 left, ref Color4 right, out Color4 result)
        {
            result.Alpha = (left.Alpha < right.Alpha) ? left.Alpha : right.Alpha;
            result.Red = (left.Red < right.Red) ? left.Red : right.Red;
            result.Green = (left.Green < right.Green) ? left.Green : right.Green;
            result.Blue = (left.Blue < right.Blue) ? left.Blue : right.Blue;
        }

        /// <summary>
        /// Returns a color containing the smallest components of the specified colors.
        /// </summary>
        /// <param name="left">The first source color.</param>
        /// <param name="right">The second source color.</param>
        /// <returns>A color containing the smallest components of the source colors.</returns>
        public static Color4 Min(Color4 left, Color4 right)
        {
            Color4 result;
            Min(ref left, ref right, out result);
            return result;
        }

        /// <summary>
        /// Adjusts the contrast of a color.
        /// </summary>
        /// <param name="value">The color whose contrast is to be adjusted.</param>
        /// <param name="contrast">The amount by which to adjust the contrast.</param>
        /// <param name="result">When the method completes, contains the adjusted color.</param>
        public static void AdjustContrast(ref Color4 value, float contrast, out Color4 result)
        {
            result.Alpha = value.Alpha;
            result.Red = 0.5f + contrast * (value.Red - 0.5f);
            result.Green = 0.5f + contrast * (value.Green - 0.5f);
            result.Blue = 0.5f + contrast * (value.Blue - 0.5f);
        }

        /// <summary>
        /// Adjusts the contrast of a color.
        /// </summary>
        /// <param name="value">The color whose contrast is to be adjusted.</param>
        /// <param name="contrast">The amount by which to adjust the contrast.</param>
        /// <returns>The adjusted color.</returns>
        public static Color4 AdjustContrast(Color4 value, float contrast)
        {
            return new Color4(                
                0.5f + contrast * (value.Red - 0.5f),
                0.5f + contrast * (value.Green - 0.5f),
                0.5f + contrast * (value.Blue - 0.5f),
                value.Alpha);
        }

        /// <summary>
        /// Adjusts the saturation of a color.
        /// </summary>
        /// <param name="value">The color whose saturation is to be adjusted.</param>
        /// <param name="saturation">The amount by which to adjust the saturation.</param>
        /// <param name="result">When the method completes, contains the adjusted color.</param>
        public static void AdjustSaturation(ref Color4 value, float saturation, out Color4 result)
        {
            float grey = value.Red * 0.2125f + value.Green * 0.7154f + value.Blue * 0.0721f;

            result.Alpha = value.Alpha;
            result.Red = grey + saturation * (value.Red - grey);
            result.Green = grey + saturation * (value.Green - grey);
            result.Blue = grey + saturation * (value.Blue - grey);
        }

        /// <summary>
        /// Adjusts the saturation of a color.
        /// </summary>
        /// <param name="value">The color whose saturation is to be adjusted.</param>
        /// <param name="saturation">The amount by which to adjust the saturation.</param>
        /// <returns>The adjusted color.</returns>
        public static Color4 AdjustSaturation(Color4 value, float saturation)
        {
            float grey = value.Red * 0.2125f + value.Green * 0.7154f + value.Blue * 0.0721f;

            return new Color4(                
                grey + saturation * (value.Red - grey),
                grey + saturation * (value.Green - grey),
                grey + saturation * (value.Blue - grey),
                value.Alpha);
        }

        /// <summary>
        /// Computes the premultiplied value of the provided color.
        /// </summary>
        /// <param name="value">The non-premultiplied value.</param>
        /// <param name="result">The premultiplied result.</param>
        public static void Premultiply(ref Color4 value, out Color4 result)
        {
            result.Alpha = value.Alpha;
            result.Red = value.Red * value.Alpha;
            result.Green = value.Green * value.Alpha;
            result.Blue = value.Blue * value.Alpha;
        }

        /// <summary>
        /// Computes the premultiplied value of the provided color.
        /// </summary>
        /// <param name="value">The non-premultiplied value.</param>
        /// <returns>The premultiplied result.</returns>
        public static Color4 Premultiply(Color4 value)
        {
            Color4 result;
            Premultiply(ref value, out result);
            return result;
        }

        /// <summary>
        /// Adds two colors.
        /// </summary>
        /// <param name="left">The first color to add.</param>
        /// <param name="right">The second color to add.</param>
        /// <returns>The sum of the two colors.</returns>
        public static Color4 operator +(Color4 left, Color4 right)
        {
            return new Color4(left.Red + right.Red, left.Green + right.Green, left.Blue + right.Blue, left.Alpha + right.Alpha);
        }

        /// <summary>
        /// Assert a color (return it unchanged).
        /// </summary>
        /// <param name="value">The color to assert (unchanged).</param>
        /// <returns>The asserted (unchanged) color.</returns>
        public static Color4 operator +(Color4 value)
        {
            return value;
        }

        /// <summary>
        /// Subtracts two colors.
        /// </summary>
        /// <param name="left">The first color to subtract.</param>
        /// <param name="right">The second color to subtract.</param>
        /// <returns>The difference of the two colors.</returns>
        public static Color4 operator -(Color4 left, Color4 right)
        {
            return new Color4(left.Red - right.Red, left.Green - right.Green, left.Blue - right.Blue, left.Alpha - right.Alpha);
        }

        /// <summary>
        /// Negates a color.
        /// </summary>
        /// <param name="value">The color to negate.</param>
        /// <returns>A negated color.</returns>
        public static Color4 operator -(Color4 value)
        {
            return new Color4(-value.Red, -value.Green, -value.Blue, -value.Alpha);
        }

        /// <summary>
        /// Scales a color.
        /// </summary>
        /// <param name="scale">The factor by which to scale the color.</param>
        /// <param name="value">The color to scale.</param>
        /// <returns>The scaled color.</returns>
        public static Color4 operator *(float scale, Color4 value)
        {
            return new Color4(value.Red * scale, value.Green * scale, value.Blue * scale, value.Alpha * scale);
        }

        /// <summary>
        /// Scales a color.
        /// </summary>
        /// <param name="value">The factor by which to scale the color.</param>
        /// <param name="scale">The color to scale.</param>
        /// <returns>The scaled color.</returns>
        public static Color4 operator *(Color4 value, float scale)
        {
            return new Color4(value.Red * scale, value.Green * scale, value.Blue * scale, value.Alpha * scale);
        }

        /// <summary>
        /// Modulates two colors.
        /// </summary>
        /// <param name="left">The first color to modulate.</param>
        /// <param name="right">The second color to modulate.</param>
        /// <returns>The modulated color.</returns>
        public static Color4 operator *(Color4 left, Color4 right)
        {
            return new Color4(left.Red * right.Red, left.Green * right.Green, left.Blue * right.Blue, left.Alpha * right.Alpha);
        }

        /// <summary>
        /// Tests for equality between two objects.
        /// </summary>
        /// <param name="left">The first value to compare.</param>
        /// <param name="right">The second value to compare.</param>
        /// <returns><c>true</c> if <paramref name="left"/> has the same value as <paramref name="right"/>; otherwise, <c>false</c>.</returns>
        [MethodImpl((MethodImplOptions)0x100)] // MethodImplOptions.AggressiveInlining
        public static bool operator ==(Color4 left, Color4 right)
        {
            return left.Equals(ref right);
        }

        /// <summary>
        /// Tests for inequality between two objects.
        /// </summary>
        /// <param name="left">The first value to compare.</param>
        /// <param name="right">The second value to compare.</param>
        /// <returns><c>true</c> if <paramref name="left"/> has a different value than <paramref name="right"/>; otherwise, <c>false</c>.</returns>
        [MethodImpl((MethodImplOptions)0x100)] // MethodImplOptions.AggressiveInlining
        public static bool operator !=(Color4 left, Color4 right)
        {
            return !left.Equals(ref right);
        }
        
        /// <summary>
        /// Performs an explicit conversion from <see cref="Color4"/> to <see cref="Vector3"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static explicit operator Vector3(Color4 value)
        {
            return new Vector3(value.Red, value.Green, value.Blue);
        }

        /// <summary>
        /// Performs an implicit conversion from <see cref="Color4"/> to <see cref="Vector4"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static implicit operator Vector4(Color4 value)
        {
            return new Vector4(value.Red, value.Green, value.Blue, value.Alpha);
        }

        /// <summary>
        /// Performs an explicit conversion from <see cref="Vector3"/> to <see cref="Color4"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static explicit operator Color4(Vector3 value)
        {
            return new Color4(value.X, value.Y, value.Z, 1.0f);
        }

        /// <summary>
        /// Performs an explicit conversion from <see cref="Vector4"/> to <see cref="Color4"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static explicit operator Color4(Vector4 value)
        {
            return new Color4(value.X, value.Y, value.Z, value.W);
        }

        /// <summary>
        /// Performs an explicit conversion from <see cref="Vector3"/> to <see cref="Color4"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static explicit operator Color4(ColorBGRA value)
        {
            return new Color4(value.R, value.G, value.B, value.A);
        }

        /// <summary>
        /// Performs an explicit conversion from <see cref="Vector4"/> to <see cref="Color4"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>The result of the conversion.</returns>
        public static explicit operator ColorBGRA(Color4 value)
        {
            return new ColorBGRA(value.Red, value.Green, value.Blue, value.Alpha);
        }

        /// <summary>
        /// Performs an explicit conversion from <see cref="Color4"/> to <see cref="System.Int32"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>
        /// The result of the conversion.
        /// </returns>
        public static explicit operator int(Color4 value)
        {
            return value.ToRgba();
        }

        /// <summary>
        /// Performs an explicit conversion from <see cref="System.Int32"/> to <see cref="Color4"/>.
        /// </summary>
        /// <param name="value">The value.</param>
        /// <returns>
        /// The result of the conversion.
        /// </returns>
        public static explicit operator Color4(int value)
        {
            return new Color4(value);
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public override string ToString()
        {
            return ToString(CultureInfo.CurrentCulture);
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <param name="format">The format to apply to each channel (float).</param>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public string ToString(string format)
        {
            return ToString(format, CultureInfo.CurrentCulture);
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <param name="formatProvider">The format provider.</param>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public string ToString(IFormatProvider formatProvider)
        {
            return string.Format(formatProvider, toStringFormat, Alpha, Red, Green, Blue);
        }

        /// <summary>
        /// Returns a <see cref="System.String"/> that represents this instance.
        /// </summary>
        /// <param name="format">The format to apply to each channel (float).</param>
        /// <param name="formatProvider">The format provider.</param>
        /// <returns>
        /// A <see cref="System.String"/> that represents this instance.
        /// </returns>
        public string ToString(string format, IFormatProvider formatProvider)
        {
            if (format == null)
                return ToString(formatProvider);

            return string.Format(formatProvider,
                                 toStringFormat,
                                 Alpha.ToString(format, formatProvider),
                                 Red.ToString(format, formatProvider),
                                 Green.ToString(format, formatProvider),
                                 Blue.ToString(format, formatProvider));
        }

        /// <summary>
        /// Returns a hash code for this instance.
        /// </summary>
        /// <returns>
        /// A hash code for this instance, suitable for use in hashing algorithms and data structures like a hash table. 
        /// </returns>
        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = Red.GetHashCode();
                hashCode = (hashCode * 397) ^ Green.GetHashCode();
                hashCode = (hashCode * 397) ^ Blue.GetHashCode();
                hashCode = (hashCode * 397) ^ Alpha.GetHashCode();
                return hashCode;
            }
        }

        /// <summary>
        /// Determines whether the specified <see cref="Color4"/> is equal to this instance.
        /// </summary>
        /// <param name="other">The <see cref="Color4"/> to compare with this instance.</param>
        /// <returns>
        /// <c>true</c> if the specified <see cref="Color4"/> is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        [MethodImpl((MethodImplOptions)0x100)] // MethodImplOptions.AggressiveInlining
        public bool Equals(ref Color4 other)
        {
            return Alpha == other.Alpha && Red == other.Red && Green == other.Green && Blue == other.Blue;
        }

        /// <summary>
        /// Determines whether the specified <see cref="Color4"/> is equal to this instance.
        /// </summary>
        /// <param name="other">The <see cref="Color4"/> to compare with this instance.</param>
        /// <returns>
        /// <c>true</c> if the specified <see cref="Color4"/> is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        [MethodImpl((MethodImplOptions)0x100)] // MethodImplOptions.AggressiveInlining
        public bool Equals(Color4 other)
        {
            return Equals(ref other);
        }

        /// <summary>
        /// Determines whether the specified <see cref="System.Object"/> is equal to this instance.
        /// </summary>
        /// <param name="value">The <see cref="System.Object"/> to compare with this instance.</param>
        /// <returns>
        /// <c>true</c> if the specified <see cref="System.Object"/> is equal to this instance; otherwise, <c>false</c>.
        /// </returns>
        public override bool Equals(object value)
        {
            if (!(value is Color4))
                return false;

            var strongValue = (Color4)value;
            return Equals(ref strongValue);
        }
    }
}
