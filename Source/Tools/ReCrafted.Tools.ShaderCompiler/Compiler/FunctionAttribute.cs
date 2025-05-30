﻿// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using ReCrafted.Tools.Common.CodeTokenizer;

namespace ReCrafted.Tools.ShaderCompiler.Compiler
{
    public class FunctionAttribute
    {
        public string AttributeName { get; set; }
        public Token[] Parameters { get; set; }
    }
}
