// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

using ReCrafted.ShaderCompiler.Tokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public class FunctionAttribute
    {
        public string AttributeName { get; set; }
        public Token[] Parameters { get; set; }
    }
}
