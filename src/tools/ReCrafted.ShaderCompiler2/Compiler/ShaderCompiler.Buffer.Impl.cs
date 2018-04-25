// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;
using ReCrafted.ShaderCompiler.Description;
using ReCrafted.CodeTokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public partial class ShaderCompiler
    {
        private void ParseBuffer()
        {
            var bufferName = _parser.ExpectToken(TokenType.Identifier);

            // construct uniform buffer
            var buffer = new ShaderUniformBuffer
            {
                Name = bufferName.Value
            };

            if (Options.Current.Verbose)
                Console.WriteLine("Processing buffer '" + bufferName.Value + "'.");

            // consume ': register(X)'
            _parser.ExpectToken(TokenType.Colon); // :
            _parser.ExpectToken(TokenType.Identifier); // register
            _parser.ExpectToken(TokenType.LeftParent); // (
            buffer.Index = int.Parse(_parser.ExpectToken(TokenType.Identifier).Value.Remove(0, 1)); // laaazy
            _parser.ExpectToken(TokenType.RightParent); // )

            // expect {
            _parser.ExpectToken(TokenType.LeftCurlyBrace);
            
            // process attribute
            if (_lastAttribute != null && _lastAttribute.Parameters.Length > 0)
            {
                // process attribute
                var targets = new List<string>();

                foreach (var token in _lastAttribute.Parameters)
                {
                    if (token.Type != TokenType.Identifier)
                        throw new Exception("Invalid parameter type passed (" + token.Value + ") to target at line " + _parser.CurrentLine);

                    targets.Add(token.Value);
                }

                buffer.Targets = targets.ToArray();
            }

            // parse all variables
            var uniforms = new List<ShaderUniform>();
            var uniformIndex = 0;
            var done = false;
            do
            {
                var token = _parser.NextToken();
                switch (token.Type)
                {
                    case TokenType.Identifier:
                        var uniformType = token.Value;
                        var uniformName = _parser.ExpectToken(TokenType.Identifier).Value;
                        var uniformSize = D3DHelper.TypeToSize(uniformType);

                        var uniform = new ShaderUniform
                        {
                            Name = uniformName,
                            Size = uniformSize,
                            Index = uniformIndex
                        };

                        uniforms.Add(uniform);

                        // and for the end, expect semicolon at the end
                        _parser.ExpectToken(TokenType.SemiColon);
                        uniformIndex++;
                        break;
                    case TokenType.RightCurlyBrace:
                        done = true;
                        break;

                }
            } while (!done);

            // add all uniforms
            buffer.Uniforms = uniforms.ToArray();

            // add buffer
            Buffers.Add(buffer);
        }
    }
}
