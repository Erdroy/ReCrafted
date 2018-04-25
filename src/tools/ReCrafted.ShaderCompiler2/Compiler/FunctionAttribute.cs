// ReCrafted (c) 2016-2018 Always Too Late

using ReCrafted.CodeTokenizer;

namespace ReCrafted.ShaderCompiler.Compiler
{
    public class FunctionAttribute
    {
        public string AttributeName { get; set; }
        public Token[] Parameters { get; set; }
    }
}
