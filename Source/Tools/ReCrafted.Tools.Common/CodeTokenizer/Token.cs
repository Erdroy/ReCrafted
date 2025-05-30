﻿// Copyright (c) 2010-2014 SharpDX - Alexandre Mutel
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

using System;

namespace ReCrafted.Tools.Common.CodeTokenizer
{
    /// <summary>
    /// Contains information about a token language.
    /// </summary>
    public class Token
    {
        public Token()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="Token" /> struct.
        /// </summary>
        /// <param name="type">The type.</param>
        /// <param name="value">The value.</param>
        public Token(TokenType type, string value)
        {
            Type = type;
            Value = value;
        }

        /// <summary>
        /// The type of the token.
        /// </summary>
        public TokenType Type;

        /// <summary>
        /// Value of the token.
        /// </summary>
        public string Value;

        public bool EqualString(string value, bool caseSensitive = false)
        {
            return string.Compare(Value, value, caseSensitive ? StringComparison.Ordinal : StringComparison.OrdinalIgnoreCase) == 0;
        }

        public override string ToString()
        {
            return string.Format("{{{0}/{1}}}", Type, Value);
        }
    }
}
