// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ReCrafted.Tools.APIGenerator.Descriptions
{
    public enum PropertyMode
    {
        Getter,
        Setter,
        GetterSetter,
    }

    public class PropertyDescription
    {
        public string Name { get; set; }
        public TypeDescription Type { get; set; }
        public string Comment { get; set; }
        public AccessModifier Access { get; set; }
        public PropertyMode Mode { get; set; }
        public List<string> Modifiers { get; } = new List<string>();

        public bool IsStatic => Modifiers.Contains("static");

        public bool ForceByValue { get; set; }

        public bool HasGetter => Mode == PropertyMode.GetterSetter || Mode == PropertyMode.Getter;
        public bool HasSetter => Mode == PropertyMode.GetterSetter || Mode == PropertyMode.Setter;

        public string GetModifiers()
        {
            var str = new StringBuilder();
            str.Append(Access.ToString().ToLower());

            var modifiers = string.Join(" ", Modifiers.SkipWhile(x => x == "noprefix"));

            if (modifiers.Length > 0)
            {
                str.Append(' ');
                str.Append(modifiers);
            }

            return str.ToString();
        }
    }
}
