// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

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

                default:
                    return baseType;
            }
        }
    }
}
