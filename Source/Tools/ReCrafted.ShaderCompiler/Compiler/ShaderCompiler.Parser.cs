// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using ReCrafted.CodeTokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public partial class ShaderCompiler
    {
        
        private void HandleIdentifier(Token token)
        {
            switch (token.Value)
            {
                case "pass":
                    _parser.IgnoreSource(true);
                    ParsePass();
                    _parser.IgnoreSource(false);
                    break;
                case "cbuffer":
                    ParseBuffer();
                    break;
                case "Texture2D":
                case "Texture3D":
                case "SamplerState":
                    ParseUniform(token.Value);
                    break;
            }
        }

        private void HandleAttribute()
        {
            var token = _parser.ExpectToken(TokenType.Identifier);
            var arguments = ParseFunctionArguments();

            // save last attribute
            _lastAttribute = new FunctionAttribute
            {
                AttributeName = token.Value,
                Parameters = arguments.ToArray()
            };

            _parser.ExpectToken(TokenType.RightBracket);
        }

        private void HandlePreprocessor()
        {
            var nextToken = _parser.NextToken();

            switch (nextToken.Value)
            {
                case "include":
                {
                    // parse include
                    nextToken = _parser.NextToken();
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
                    nextToken = _parser.NextToken();
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
                    nextToken = _parser.NextToken();
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

            _parser.ExpectToken(TokenType.LeftParent);

            // expect series of arguments...
            var done = false;
            do
            {
                var token = _parser.NextToken();

                if (token.Type == TokenType.Identifier || token.Type == TokenType.Number)
                {
                    arguments.Add(token);

                    // check next token
                    var nextToken = _parser.NextToken();
                    switch (nextToken.Type)
                    {
                        case TokenType.Comma:
                            break;
                        case TokenType.RightParent:
                            done = true;
                            break;

                        default:
                            throw new Exception("Expected right parent or next argument line " + _parser.CurrentLine + ", but got " + token.Type + ".");
                    }
                }
            }
            while (!done);

            // return arguments
            return arguments;
        }
    }
}
