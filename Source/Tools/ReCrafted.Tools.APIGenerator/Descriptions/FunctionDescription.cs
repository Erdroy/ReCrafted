// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Collections.Generic;
using System.Text;

namespace ReCrafted.Tools.APIGenerator.Descriptions
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
        public bool Extern { get; set; } = false;
        public bool IsStatic => Modifiers.Contains("static");
        public List<string> Modifiers { get; } = new List<string>();
        public List<Param> Parameters { get; } = new List<Param>();

        public string GetCPlusPlusParametersRedirect()
        {
            var paramString = new StringBuilder();

            foreach (var parameter in Parameters)
            {
                if (paramString.Length != 0)
                    paramString.Append(", ");

                paramString.Append(parameter.Name);
            }

            return paramString.ToString();
        }

        public string GetCSharpParametersRedirect()
        {
            var paramString = new StringBuilder();

            if (!IsStatic)
            {
                // Add native object pointer forward from Object::NativePtr
                paramString.Append("NativePtr");
            }

            foreach (var parameter in Parameters)
            {
                if (paramString.Length != 0)
                    paramString.Append(", ");

                if(parameter.Type.PassByReference)
                    paramString.Append("ref ");

                paramString.Append(parameter.Name);
            }

            return paramString.ToString();
        }

        public string GetCPlusPlusParameters(ClassDescription classDesc, bool withObjectPtr = false)
        {
            var paramString = new StringBuilder();

            if (!IsStatic && withObjectPtr)
            {
                // Add native object pointer
                paramString.Append(classDesc.Name);
                paramString.Append('*');
                paramString.Append(' ');
                paramString.Append("instance");
            }

            foreach (var parameter in Parameters)
            {
                if (paramString.Length != 0)
                    paramString.Append(", ");

                if (parameter.Type.IsSpecial)
                    paramString.Append(parameter.Type.ToSpecial());
                else
                    paramString.Append(parameter.Type.ToString());

                paramString.Append(' ');
                if (parameter.Type.IsSpecial)
                    paramString.Append($"p_{parameter.Name}");
                else
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
                paramString.Append("System.IntPtr");
                paramString.Append(' ');
                paramString.Append("nativePtr");
            }

            foreach (var parameter in Parameters)
            {
                if(paramString.Length != 0)
                    paramString.Append(", ");

                paramString.Append(parameter.Type.ToCSharp());
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
