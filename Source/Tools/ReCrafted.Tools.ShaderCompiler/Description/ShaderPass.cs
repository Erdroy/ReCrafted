// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.Tools.ShaderCompiler.Description
{
    [Serializable]
    public class ShaderPass
    {
        public string Name { get; set; }
        public int Profile { get; set; }

        public string VSFunction { get; set; }
        public string PSFunction { get; set; }
        public string CSFunction { get; set; }
        public string GSFunction { get; set; }

        public byte[] VSByteCode { get; set; }
        public byte[] PSByteCode { get; set; }
        public byte[] CSByteCode { get; set; }
        public byte[] GSByteCode { get; set; }
    }
}
