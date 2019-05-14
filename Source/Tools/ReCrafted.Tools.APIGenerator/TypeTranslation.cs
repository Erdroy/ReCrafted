// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System.Linq;
using System.Text;
using ReCrafted.Tools.APIGenerator.Descriptions;

namespace ReCrafted.Tools.APIGenerator
{
    public static class TypeTranslation
    {
        public static string TranslateBaseType(string baseType)
        {
            switch (baseType)
            {
                case "uint8_t":
                    return "byte";
                case "int8_t":
                    return "sbyte";

                case "uint16_t":
                    return "ushort";
                case "int16_t":
                    return "short";

                case "uint32_t":
                    return "uint";
                case "int32_t":
                    return "int";

                case "uint64_t":
                    return "System.UInt64"; // or ulong...?
                case "int64_t":
                    return "System.Int64";

                case "MonoObject":
                    return "object";
                case "MonoType":
                    return "System.IntPtr";

                case "Char":
                    return "char";

                default:
                    return baseType;
            }
        }

        public static string ToCSharpAction(TypeDescription type)
        {
            var sb = new StringBuilder();

            if (type.GenericTypes.First().IsVoid)
            {
                // This is action
                sb.Append("System.Action<");

                for (var i = 1; i < type.GenericTypes.Count; i++)
                {
                    if (i != 1)
                        sb.Append(", ");

                    sb.Append(type.GenericTypes[i].ToCSharp());
                }

                sb.Append(">");
            }
            else
            {
                // This is func
                sb.Append("System.Func<");

                for (var i = 1; i < type.GenericTypes.Count; i++)
                {
                    sb.Append(type.GenericTypes[i].ToCSharp());
                    sb.Append(", ");
                }

                // The last type is the return type of the function delegate.
                sb.Append(type.GenericTypes.First().ToCSharp());

                sb.Append(">");
            }

            return sb.ToString();
        }

        public static string ToCSharpArray(TypeDescription type)
        {
            var sb = new StringBuilder();
            sb.Append(type.GenericTypes.First().ToCSharp());
            sb.Append("[]");
            return sb.ToString();
        }
    }
}
