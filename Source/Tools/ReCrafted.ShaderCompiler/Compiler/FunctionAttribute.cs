// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.CodeTokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public class FunctionAttribute
    {
        public string AttributeName { get; set; }
        public Token[] Parameters { get; set; }
    }
}
