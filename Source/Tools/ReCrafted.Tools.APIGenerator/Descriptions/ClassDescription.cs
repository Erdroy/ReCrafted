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
        public string[] Comment { get; set; }
        public AccessModifier Access { get; set; }
        public List<string> Modifiers { get; } = new List<string>();
        public List<string> GenericTypes { get; } = new List<string>();

        public bool IsGeneric => GenericTypes.Count > 0;
        public bool IsStatic => Modifiers.Contains("static");
        public bool IsAbstract => Modifiers.Contains("abstract");

        public string GetGenerics()
        {
            if (!IsGeneric)
                return "";

            var str = new StringBuilder();
            str.Append('<');
            foreach (var generic in GenericTypes)
            {
                if (str.Length > 1)
                {
                    str.Append(',');
                    str.Append(' ');
                }

                str.Append(generic);
            }
            str.Append('>');
            return str.ToString();
        }

        public string GetInheritance()
        {
            if (string.IsNullOrEmpty(Inherits))
                return "";

            return $" : {Inherits}";
        }

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
