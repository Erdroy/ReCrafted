// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

using ReCrafted.ShaderCompiler.Tokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
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
                    var name = ExpectToken(TokenType.Identifier);
                    Textures2D.Add(name.Value);
                    break;
                }
                case "Texture3D":
                {
                    var name = ExpectToken(TokenType.Identifier);
                    Textures3D.Add(name.Value);
                    break;
                }

                case "SamplerState":
                {
                    ExpectToken(TokenType.LessThan);
                    IgnoreSource(true);
                    var type = ExpectToken(TokenType.Identifier);
                    ExpectToken(TokenType.GreaterThan);
                    IgnoreSource(false);

                    Samplers.Add(type.Value);
                    break;
                }
            }
        }
    }
}
