// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

using System;

namespace ReCrafted.ShaderCompiler.Description
{
    [Serializable]
    public class ShaderDescription
    {
        public string Name { get; set; }
        public string Description { get; set; }
        public string[] SamplerStates { get; set; }
        public string[] Textures2D { get; set; }
        public string[] Textures3D { get; set; }
        public ShaderUniformBuffer[] UniformBuffers { get; set; }
        public ShaderUniform[] Uniforms { get; set; }
        public ShaderPass[] Passes { get; set; }
    }
}
