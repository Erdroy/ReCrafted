// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.Linq;
using ReCrafted.ShaderCompiler.Tokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public partial class ShaderCompiler
    {
        private Token NextToken(bool includeInvalid = false)
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

                    if (includeInvalid)
                        return token;

                    continue;
                }

                if (!includeInvalid)
                {
                    if (token.Type == TokenType.Whitespace
                        || token.Type == TokenType.CommentMultiLine
                        || token.Type == TokenType.CommentSingleLine)
                    {
                        // set previous token
                        _previousToken = _tokenEnumerator.Current;
                        continue;
                    }
                }

                return token;
            }

            // this is the end...
            return new Token(TokenType.EndOfFile, "", new SourceSpan());
        }

        private Token ExpectTokens(TokenType[] tokenTypes, bool includeInvalid = false)
        {
            var token = NextToken(includeInvalid);

            if (tokenTypes.Contains(token.Type))
                return token;

            throw new Exception("Expected " + string.Join(" or ", tokenTypes) + " at line " + _currentLineNumber + ", but got " + token.Type + ".");
        }

        private Token ExpectToken(TokenType tokenType, bool includeInvalid = false)
        {
            var token = NextToken(includeInvalid);

            if (token.Type == tokenType)
                return token;

            throw new Exception("Expected " + tokenType + " at line " + _currentLineNumber + ", but got " + token.Type + ".");
        }

        private void HandleIdentifier(Token token)
        {
            switch (token.Value)
            {
                case "pass":
                    IgnoreSource(true);
                    ParsePass();
                    IgnoreSource(false);
                    break;
                case "cbuffer":
                    ParseBuffer();
                    break;
                case "Texture2D":
                case "Texture3D":
                case "SamplerState":
                    ParseUniform();
                    break;
            }
        }

        private void HandleAttribute()
        {
            var token = ExpectToken(TokenType.Identifier);
            var arguments = ParseFunctionArguments();

            // save last attribute
            _lastAttribute = new FunctionAttribute
            {
                AttributeName = token.Value,
                Parameters = arguments.ToArray()
            };

            ExpectToken(TokenType.RightBracket);
        }

        private void HandlePreprocessor()
        {
            var nextToken = NextToken();

            switch (nextToken.Value)
            {
                case "include":
                {
                    // parse include
                    nextToken = NextToken();
                    if (nextToken.Type == TokenType.String)
                    {
                        var fileName = nextToken.Value;
                        Includes.Add(fileName.Substring(1, fileName.Length - 2));
                    }
                    return;
                }
                case "name":
                {
                    // parse name
                    nextToken = NextToken();
                    if (nextToken.Type == TokenType.String)
                    {
                        var name = nextToken.Value;
                        ShaderName = name.Substring(1, name.Length - 2);
                    }
                    return;
                }
                case "description":
                case "desc":
                {
                    // parse name
                    nextToken = NextToken();
                    if (nextToken.Type == TokenType.String)
                    {
                        var desc = nextToken.Value;
                        ShaderDescription = desc.Substring(1, desc.Length - 2);
                    }
                    return;
                }
            }

            // make sure that we are clean here (for example, if there will be some not processed identifiers or something)
            // preprocessor takes the whole line, so then we skip to the next new line.
            //IgnoreTokensUntil(TokenType.Newline); ?
        }

        private List<Token> ParseFunctionArguments()
        {
            var arguments = new List<Token>();

            ExpectToken(TokenType.LeftParent);

            // expect series of arguments...
            var done = false;
            do
            {
                var token = NextToken();

                if (token.Type == TokenType.Identifier || token.Type == TokenType.Number)
                {
                    arguments.Add(token);

                    // check next token
                    var nextToken = NextToken();
                    switch (nextToken.Type)
                    {
                        case TokenType.Comma:
                            break;
                        case TokenType.RightParent:
                            done = true;
                            break;

                        default:
                            throw new Exception("Expected right parent or next argument line " + _currentLineNumber + ", but got " + token.Type + ".");
                    }
                }
            }
            while (!done);

            // return arguments
            return arguments;
        }
    }
}
