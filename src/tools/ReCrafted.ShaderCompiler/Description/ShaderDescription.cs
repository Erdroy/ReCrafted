// ReCrafted (c) 2016-2018 Always Too Late

using System;

namespace ReCrafted.ShaderCompiler.Description
{
    [Serializable]
    public class ShaderDescription
    {
        public int AssetVersion { get; } = 1;
        public int AssetType { get; } = 3;
        public string AssetGuid { get; set; }

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
