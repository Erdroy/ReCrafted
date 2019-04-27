// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Collections.Generic;

namespace ReCrafted.Tools.APIGenerator.Description
{
    public class ClassDescription
    {
        public string Name { get; set; }
        public string Namespace { get; set; }
        public string Inherits { get; set; }
        public string Comment { get; set; }
        public AccessModifier Access { get; set; }
        public List<string> Modifiers { get; } = new List<string>();
    }
}
