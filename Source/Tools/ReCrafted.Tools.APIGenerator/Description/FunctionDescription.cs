// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ReCrafted.Tools.APIGenerator.Description
{
    public class FunctionDescription
    {
        public struct Param
        {
            public string Name { get; set; }
            public TypeDescription Type { get; set; }
        }

        public string Name { get; set; }
        public TypeDescription ReturnType { get; set; }
        public string Comment { get; set; }
        public AccessModifier Access { get; set; }
        public bool IsStatic => Modifiers.Contains("static");
        public List<string> Modifiers { get; } = new List<string>();
        public List<Param> Parameters { get; } = new List<Param>();

        public string GetCSharpParametersRedirect()
        {
            var paramString = new StringBuilder();

            if (!IsStatic)
            {
                // Add native object pointer forward from Object::NativePtr
                paramString.Append("nativePtr");
            }

            foreach (var parameter in Parameters)
            {
                if (paramString.Length != 0)
                    paramString.Append(", ");

                if(TypeTranslation.PassByReference(parameter.Type))
                    paramString.Append("ref ");

                paramString.Append(parameter.Name);
            }

            return paramString.ToString();
        }

        public string GetCSharpParameters(bool withObjectPtr = false)
        {
            var paramString = new StringBuilder();

            if (!IsStatic && withObjectPtr)
            {
                // Add native object pointer
                paramString.Append("IntPtr");
                paramString.Append(' ');
                paramString.Append("nativePtr");
            }

            foreach (var parameter in Parameters)
            {
                if(paramString.Length != 0)
                    paramString.Append(", ");

                var type = TypeTranslation.ToCSharp(parameter.Type);

                paramString.Append(type);
                paramString.Append(' ');
                paramString.Append(parameter.Name);
            }

            return paramString.ToString();
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
