// ReCrafted (c) 2016-2018 Always Too Late

using System;
using System.Collections.Generic;

namespace ReCrafted.ShaderCompiler.Description
{
    [Serializable]
    public class ShaderUniformBuffer
    {
        public string Name { get; set; }
        public int Index { get; set; }
        public List<string> Targets { get; set; } = new List<string>();
        public List<ShaderUniform> Uniforms { get; set; } = new List<ShaderUniform>();
    }
}
