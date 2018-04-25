// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.ShaderCompiler.Description
{
    [Serializable]
    public class ShaderUniformBuffer
    {
        public string Name { get; set; }
        public int Index { get; set; }
        public string[] Targets { get; set; }
        public ShaderUniform[] Uniforms { get; set; }
    }
}
