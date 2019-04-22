// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Collections.Generic;

namespace ReCrafted.Tools.APIGenerator.Description
{
    public class FunctionDescription
    {
        public struct Param
        {
            public string Name { get; set; }
            public NativeTypeDescription NativeType { get; set; }
        }

        public string Name { get; set; }
        public NativeTypeDescription ReturnType { get; set; }
        public string Comment { get; set; }
        public List<string> Modifiers { get; } = new List<string>();
        public List<Param> Parameters { get; } = new List<Param>();
    }
}
