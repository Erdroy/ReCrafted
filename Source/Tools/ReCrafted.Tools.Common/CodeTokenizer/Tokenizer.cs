// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

// This code is based on SharpDX toolkit code fragment
// https://github.com/sharpdx/Toolkit/blob/master/Source/Toolkit/SharpDX.Toolkit.Compiler/Effect/Tokenizer.cs
// Original license:
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

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;

namespace ReCrafted.Tools.Common.CodeTokenizer
{
    /// <summary>
    /// Parser class. Provides basic implementation for generic C-like syntax code parsing.
    /// </summary>
    public class Tokenizer : IDisposable
    {
        private static readonly Regex RegexTokenizer = new Regex(
            @"(?<ws>[ \t]+)|" +
            @"(?<nl>(?:\r\n|\n))|" +
            @"(?<commul>/\*(?:(?!\*/)(?:.|[\r\n]+))*\*/)|" +
            @"(?<comsin>//(.*?)\r?\n)|" +
            @"(?<ident>[a-zA-Z_][a-zA-Z0-9_]*)|" +
            @"(?<hex>0x[0-9a-fA-F]+)|" +
            @"(?<number>[\-\+]?\s*[0-9]*\.?[0-9]+(?:[eE][-+]?[0-9]+)?f?)|" +
            @"(?<equal>=)|" +
            @"(?<comma>,)|" +
            @"(?<semicolon>;)|" +
            @"(?<lcb>\{)|" +
            @"(?<rcb>\})|" +
            @"(?<lpar>\()|" +
            @"(?<rpar>\))|" +
            @"(?<lb>\[)|" +
            @"(?<rb>\])|" +
            @"(?<str>""[^""\\]*(?:\\.[^""\\]*)*"")|" +
            @"(?<prep>#)|" +
            @"(?<colon>:)|" +
            @"(?<doublecolon>::)|" +
            @"(?<dot>\.)|" +
            @"(?<lt>\<)|" +
            @"(?<gt>\>)|" +
            @"(?<unk>[^\s]+)",
            RegexOptions.Compiled
        );
        
        private IEnumerator<Token> _tokenEnumerator;
        private Token _previousToken;
        private bool _ignoreSource;
        private int _currentLineNumber;

        /// <summary>
        /// Tokenizes the given file (through constructor).
        /// </summary>
        /// <param name="sourceCode">The source code for this tokenizer to run on.</param>
        public void Tokenize(string sourceCode)
        {
            if (_tokenEnumerator != null)
                throw new Exception("This shader is already parsed!");

            var tokens = TokenizeInternal(sourceCode);
            _tokenEnumerator = tokens.GetEnumerator();
        }

        /// <summary>
        /// Gets next token.
        /// </summary>
        /// <param name="includeWhitespaces">When false, all white-space tokens will be ignored.</param>
        /// <param name="includeComments">When false, all comment (single line and multi-line) tokens will be ignored.</param>
        /// <returns>The token. Check for EndOfFile token-type to detect end-of-file.</returns>
        public Token NextToken(bool includeWhitespaces = false, bool includeComments = false)
        {
            // weird logic, but works ok
            while (_tokenEnumerator.MoveNext())
            {
                var token = _tokenEnumerator.Current;

                if (token == null)
                    continue;

                // construct final source code when needed
                if (!_ignoreSource && _previousToken != null)
                    ProcessedSourceCode.Append(_previousToken.Value);

                // set previous token
                _previousToken = _tokenEnumerator.Current;

                if (token.Type == TokenType.Newline)
                {
                    // add new line even when source ignore flag is being used
                    if (_ignoreSource)
                    {
                        ProcessedSourceCode.Append(token.Value);
                        _previousToken = null;
                    }

                    _currentLineNumber++;

                    if (includeWhitespaces)
                        return token;

                    continue;
                }

                if (!includeWhitespaces && token.Type == TokenType.Whitespace)
                {
                    if (!includeComments && (token.Type == TokenType.CommentMultiLine || token.Type == TokenType.CommentSingleLine))
                    {
                        // set previous token
                        _previousToken = _tokenEnumerator.Current;
                        continue;
                    }

                    // set previous token
                    _previousToken = _tokenEnumerator.Current;
                    continue;
                }

                return token;
            }

            // this is the end...
            return new Token(TokenType.EndOfFile, "");
        }

        /// <summary>
        /// Expects any token of given types. Throws <see cref="Exception"/> when token is not found.
        /// </summary>
        /// <param name="tokenTypes">The allowed token types.</param>
        /// <param name="includeWhitespaces">When false, all white-space tokens will be ignored.</param>
        /// <param name="includeComments">When false, all comment (single line and multi-line) tokens will be ignored.</param>
        /// <returns>The found token.</returns>
        public Token ExpectAnyTokens(TokenType[] tokenTypes, bool includeWhitespaces = false, bool includeComments = false)
        {
            var token = NextToken(includeWhitespaces, includeComments);

            if (tokenTypes.Contains(token.Type))
                return token;

            throw new Exception("Expected " + string.Join(" or ", tokenTypes) + " at line " + _currentLineNumber + ", but got " + token.Type + ".");
        }

        /// <summary>
        /// Expects token of given types in the same order. Throws <see cref="Exception"/> when token is not found.
        /// </summary>
        /// <param name="tokenTypes">The allowed token types.</param>
        /// <param name="includeWhitespaces">When false, all white-space tokens will be ignored.</param>
        /// <param name="includeComments">When false, all comment (single line and multi-line) tokens will be ignored.</param>
        /// <returns>The found token.</returns>
        public void ExpectAllTokens(TokenType[] tokenTypes, bool includeWhitespaces = false, bool includeComments = false)
        {
            foreach (var tokenType in tokenTypes)
            {
                var token = NextToken(includeWhitespaces, includeComments);

                if(token.Type != tokenType)
                    throw new Exception(
                        $"Expected {tokenType} at line {_currentLineNumber}, but got {token.Type}.");
            }
        }

        /// <summary>
        /// Expects any token of given type. Throws <see cref="Exception"/> when token is not found.
        /// </summary>
        /// <param name="tokenType">The only allowed token type.</param>
        /// <param name="includeWhitespaces">When false, all white-space tokens will be ignored.</param>
        /// <param name="includeComments">When false, all comment (single line and multi-line) tokens will be ignored.</param>
        /// <returns>The found token.</returns>
        public Token ExpectToken(TokenType tokenType, bool includeWhitespaces = false, bool includeComments = false)
        {
            var token = NextToken(includeWhitespaces, includeComments);

            if (token.Type == tokenType)
                return token;

            throw new Exception("Expected " + tokenType + " at line " + _currentLineNumber + ", but got " + token.Type + ".");
        }

        /// <summary>
        /// Starts or ends source code building.
        /// </summary>
        /// <param name="ignore">When true, code will be ignored.</param>
        public void IgnoreSource(bool ignore)
        {
            _ignoreSource = ignore;

            if (!ignore)
                _previousToken = null;
        }

        /// <summary>
        /// Skips all tokens until the tokenizer steps into token of given type 
        /// (and it is also skipped, so, NextToken will give the next token).
        /// </summary>
        /// <param name="tokenType">The expected token type.</param>
        public void SkipUntil(TokenType tokenType)
        {
            do { } while (NextToken(true).Type != tokenType);
        }

        /// <summary>
        /// Disposes the <see cref="Tokenizer"/>.
        /// </summary>
        public void Dispose()
        {
            ProcessedSourceCode?.Clear();
            _tokenEnumerator?.Dispose();
        }
        
        private IEnumerable<Token> TokenizeInternal(string input)
        {
            var matches = RegexTokenizer.Matches(input);
            foreach (Match match in matches)
            {
                var i = 0;
                foreach (Group group in match.Groups)
                {
                    var matchValue = group.Value;

                    if (group.Success && i > 1)
                    {
                        yield return new Token
                        {
                            Type = (TokenType)(i - 2),
                            Value = matchValue
                        };
                    }

                    i++;
                }
            }
        }

        /// <summary>
        /// The processed source code with all ignores excluded (<see cref="IgnoreSource"/>).
        /// </summary>
        public StringBuilder ProcessedSourceCode { get; } = new StringBuilder();

        /// <summary>
        /// The current token.
        /// </summary>
        public Token CurrentToken => _tokenEnumerator?.Current;

        /// <summary>
        /// The current line in source code. Not implemented.
        /// </summary>
        public int CurrentLine => 0; // TODO: Implement proper line counting.
    }
}
