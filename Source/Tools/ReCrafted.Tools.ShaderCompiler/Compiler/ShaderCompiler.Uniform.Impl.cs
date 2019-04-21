// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.Tools.CodeTokenizer;

namespace ReCrafted.Tools.ShaderCompiler.Compiler
{
    public partial class ShaderCompiler
    {
        private void ParseUniform(string uniformType)
        {
            // parse Textures, Samplers and other uniforms
            switch (uniformType)
            {
                case "Texture2D":
                {
                    var name = _parser.ExpectToken(TokenType.Identifier);
                    Textures2D.Add(name.Value);
                    break;
                }
                case "Texture3D":
                {
                    var name = _parser.ExpectToken(TokenType.Identifier);
                    Textures3D.Add(name.Value);
                    break;
                }

                case "SamplerState":
                {
                    _parser.ExpectToken(TokenType.LessThan);
                    _parser.IgnoreSource(true);
                    var type = _parser.ExpectToken(TokenType.Identifier);
                    _parser.ExpectToken(TokenType.GreaterThan);
                    _parser.IgnoreSource(false);

                    Samplers.Add(type.Value);
                    break;
                }
            }
        }
    }
}
