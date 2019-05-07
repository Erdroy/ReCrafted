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

        public string GetterFunctionName { get; set; }
        public string SetterFunctionName { get; set; }

        public bool IsStatic => Modifiers.Contains("static");

        public bool ForceByValue { get; set; }

        public bool HasGetter => Mode == PropertyMode.GetterSetter || Mode == PropertyMode.Getter;
        public bool HasSetter => Mode == PropertyMode.GetterSetter || Mode == PropertyMode.Setter;

        public string GetCPlusPlusParameter(ClassDescription classDesc, bool isReturn)
        {
            var paramString = new StringBuilder();

            if (!IsStatic)
            {
                // Add native object pointer
                paramString.Append(classDesc.Name);
                paramString.Append('*');
                paramString.Append(' ');
                paramString.Append("instance");
                paramString.Append(", ");
            }

            if (Type.IsSpecial)
                paramString.Append(Type.ToSpecial());
            else
                paramString.Append(Type.ToString(isReturn, false, false));

            paramString.Append('*');

            paramString.Append(' ');
            if (Type.IsSpecial)
                paramString.Append("p_data");
            else
                paramString.Append("data");

            return paramString.ToString();
        }

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
