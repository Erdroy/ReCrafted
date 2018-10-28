// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.ShaderCompiler.Description
{
    [Serializable]
    public class ShaderUniform
    {
        public string Name { get; set; }
        public int Size { get; set; }
        public int Length { get; set; } = 1;
        public int Index { get; set; }
    }
}
