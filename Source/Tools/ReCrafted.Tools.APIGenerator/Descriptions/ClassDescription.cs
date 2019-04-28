// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Collections.Generic;
using System.Text;

namespace ReCrafted.Tools.APIGenerator.Descriptions
{
    public class ClassDescription
    {
        public string Name { get; set; }
        public string Namespace { get; set; }
        public string Inherits { get; set; }
        public string Comment { get; set; }
        public AccessModifier Access { get; set; }
        public List<string> Modifiers { get; } = new List<string>();

        public string GetModifiers()
        {
            var str = new StringBuilder();
            str.Append(Access.ToString().ToLower());

            var modifiers = string.Join(" ", Modifiers);

            if (modifiers.Length > 0)
            {
                str.Append(' ');
                str.Append(modifiers);
            }

            return str.ToString();
        }
    }
}
