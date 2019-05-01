// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using ReCrafted.Tools.APIGenerator.Descriptions;

namespace ReCrafted.Tools.APIGenerator
{
    public static class TypeTranslation
    {
        // TODO: Proper function parameter handling
        // const Type&, Type&, Type* etc. support is required.
        // Example:
        // 'const Vector3*', 'Vector3*' should become 'Vector3', and C++ should generate 'Vector3*' - this is because we cannot check the type
        // 'Vector3&' and 'const Vector3&' should become 'ref Vector3', and C++ should generate 'Vector3*'

        // TODO: Common type conversion: strings, integers etc.
        // Because: 'const char*', 'const String&' becomes string,
        // uint32_t becomes uint etc.

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

        public static string ToCSharp(TypeDescription cppType)
        {
            var baseType = TranslateBaseType(cppType.BaseType);
            
            // Handle string type
            switch (cppType.BaseType)
            {
                case "char" when cppType.ByPtr: // c string
                case "std::string":             // std string
                case "String":                  // Engine's string
                    return "string";
            }

            // TODO: Handle arrays
            // - parser will need additional functionality to support the square bracket thingies
            // Type<Type1>
            // lib::Type
            // lib::Type<Type1>
            // lib::Type<lib::Type1>

            // Type&        -> ref Type (struct reference)
            // const Type&  -> Type     (struct reference)
            // Type*        -> Type     (object/class reference)
            // const Type*  -> NOT SUPPORTED

            // const Type* - not supported.
            if (cppType.ByPtr && cppType.IsConst)
                throw new Exception($"Type with constant pointer is not supported ({cppType}).");

            // Handle: const Type&
            if (cppType.IsConst && cppType.ByRef)
                return baseType;

            // Handle: Type&
            if (cppType.ByRef)
                return $"ref {baseType}";

            // Handle: Type*, Type
            return baseType;
        }

        public static bool PassByReference(TypeDescription cppType)
        {
            if (cppType.IsConst && cppType.ByRef)
                return false;

            return cppType.ByRef;
        }
    }
}
