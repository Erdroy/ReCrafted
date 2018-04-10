// GFXL - Graphics Library (c) 2016-2017 Damian 'Erdroy' Korczowski

using System;

namespace ReCrafted.ShaderCompiler.Description
{
    [Serializable]
    public class ShaderUniform
    {
        public string Name { get; set; }
        public string Type { get; set; }

        public int Index { get; set; }
    }
}
