// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.Tools.ShaderCompiler.Description
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
