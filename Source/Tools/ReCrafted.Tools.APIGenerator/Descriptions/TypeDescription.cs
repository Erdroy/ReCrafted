// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;

namespace ReCrafted.Tools.APIGenerator.Descriptions
{
    public struct TypeDescription
    {
        public string BaseType { get; set; }
        public bool IsConst { get; set; }
        public bool ByRef { get; set; }
        public bool ByPtr { get; set; }

        public bool IsSpecial
        {
            get
            {
                // TODO: Handle arrays
                // - parser will need additional functionality to support the square bracket thingies
                // Type<Type1>
                // lib::Type
                // lib::Type<Type1>
                // lib::Type<lib::Type1>

                switch (BaseType)
                {
                    case "char" when ByPtr: // c string
                    case "std::string": // std string
                    case "String": // Engine's string
                        return true;
                    default:
                        return false;
                }
            }
        }

        public bool PassByReference
        {
            get
            {
                if (IsConst && ByRef)
                    return false;

                return ByRef;
            }
        }

        public string GetSpecialConversion()
        {
            switch (BaseType)
            {
                case "char" when ByPtr: // c string
                    return "MONO_STRING_TO_CSTR";
                case "std::string": // std string
                    return "MONO_STRING_TO_STDSTR";
                case "String": // Engine's string
                    return "MONO_STRING_TO_STR";
                default:
                    throw new Exception($"No special conversion is found for type {ToString()}.");
            }
        }
        
        public string ToCSharp()
        {
            var baseType = TypeTranslation.TranslateBaseType(BaseType);

            // Handle string type
            switch (BaseType)
            {
                case "char" when ByPtr: // c string
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
            if (ByPtr && IsConst)
                throw new Exception($"Type with constant pointer is not supported ({this}).");

            // Handle: const Type&
            if (IsConst && ByRef)
                return baseType;

            // Handle: Type&
            if (ByRef)
                return $"ref {baseType}";

            // Handle: Type*, Type
            return baseType;
        }

        public string ToSpecial()
        {
            switch (BaseType)
            {
                case "char" when ByPtr: // c string
                case "std::string": // std string
                case "String": // Engine's string
                    return "MonoString*";
                default:
                    return ToString();
            }
        }

        public override string ToString()
        {
            return $"{(IsConst ? "const " : "")}{BaseType}{(ByRef ? "&" : "")}{(ByPtr ? "*" : "")}";
        }
    }
}
