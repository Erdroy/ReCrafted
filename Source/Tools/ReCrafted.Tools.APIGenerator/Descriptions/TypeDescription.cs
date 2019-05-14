// ReCrafted (c) 2016-2019 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace ReCrafted.Tools.APIGenerator.Descriptions
{
    public class TypeDescription : IEquatable<TypeDescription>
    {
        public string BaseType { get; set; }
        public bool IsConst { get; set; }
        public bool ByRef { get; set; }
        public bool ByPtr { get; set; }
        public List<TypeDescription> GenericTypes { get; } = new List<TypeDescription>();

        public bool CastToManaged => ByPtr && !ByRef && !IsConst && BaseType != "MonoObject";

        public bool PassByReference
        {
            get
            {
                if (IsConst && ByRef)
                    return false;

                return ByRef;
            }
        }

        public bool IsGeneric => GenericTypes.Count > 0;
        public bool IsVoid => BaseType == "void" && !ByPtr;
        public bool IsSpecial
        {
            get
            {
                switch (BaseType)
                {
                    case "char" when ByPtr: // c string
                    case "std::string":     // std string
                    case "String":          // Engine's string
                    case "Action":          // Engine's delegate
                    case "Array":
                        return true;
                    default:
                        return false;
                }
            }
        }

        public string GetSpecialReturnConversion()
        {
            switch (BaseType)
            {
                case "char" when ByPtr: // c string
                    return "MONO_STRING_FROM_CSTR";
                case "std::string": // std string
                    return "MONO_STRING_FROM_STDSTR";
                case "String": // Engine's string
                    return "MONO_STRING_FROM_STR";
                case "Array" when GenericTypes.First().CastToManaged:
                    return "MONO_ARRAY_FROM_OBJECT_ARRAY";
                case "Array":
                    return "MONO_ARRAY_FROM_ARRAY";
                case "Action": // Engine's delegate
                    throw new Exception("Action/Func return type is not supported!");
                default:
                    throw new Exception($"No special conversion is found for type {ToString()}.");
            }
        }

        public string GetSpecialConversion(bool returnConversion = false)
        {
            if (returnConversion)
                return GetSpecialReturnConversion();

            switch (BaseType)
            {
                case "char" when ByPtr: // c string
                    return "MONO_STRING_TO_CSTR";
                case "std::string": // std string
                    // Note: We are using the same macro to convert into cstr,
                    // this will convert automatically into std::string,
                    // due ti it's constructors
                    return "MONO_STRING_TO_CSTR";
                case "String": // Engine's string
                    return "MONO_STRING_TO_STR";
                case "Array" when GenericTypes.First().CastToManaged:
                    return "MONO_ARRAY_TO_OBJECT_ARRAY";
                case "Array":
                    return "MONO_ARRAY_TO_ARRAY";
                case "Action": // Engine's delegate
                    return "MONO_DELEGATE_TO_ACTION_" + GenericTypes.Count;
                default:
                    throw new Exception($"No special conversion is found for type {ToString()}.");
            }
        }

        public string GetSpecialFree()
        {
            switch (BaseType)
            {
                case "char" when ByPtr: // c string
                case "std::string":     // std string
                    return "MONO_FREE";
                case "String":          // Engine's string
                case "Array":
                case "Action":          // Engine's delegate
                    return "MONO_FREE_STUB";
                default:
                    throw new Exception($"No special conversion is found for type {ToString()}.");
            }
        }

        public string ToCSharp(bool proxy = true, bool forceByValue = false)
        {
            var baseType = TypeTranslation.TranslateBaseType(BaseType);

            // Handle string type
            switch (BaseType)
            {
                case "void" when ByPtr: // c string
                    return "System.IntPtr";
                case "char" when ByPtr: // c string
                case "std::string":             // std string
                case "String":                  // Engine's string
                    return "string";
                case "Action":
                    return TypeTranslation.ToCSharpAction(this);
                case "Array":
                    return TypeTranslation.ToCSharpArray(this);
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
            {
                if (forceByValue)
                    return baseType;

                return $"ref {baseType}";
            }

            // Handle: Type*
            if (ByPtr && !proxy && CastToManaged)
                return "System.IntPtr";

            // Handle: Type (and Type* when not proxy, but only extern function)
            return baseType;
        }

        public string ToSpecial(bool isReturn = false)
        {
            switch (BaseType)
            {
                case "char" when ByPtr: // c string
                case "std::string": // std string
                case "String": // Engine's string
                    return "MonoString*";
                case "Action": // Engine's delegate
                    return "MonoObject*";
                case "Array":
                    return "MonoArray*";
                default:
                    return ToString(isReturn);
            }
        }

        public string ConstructSpecialConversion(bool returnConversion, string valueVariableName)
        {
            var sb = new StringBuilder(GetSpecialConversion(returnConversion));
            sb.Append("(");
            sb.Append(valueVariableName);

            if (IsGeneric)
            {
                var id = 0;
                foreach (var genericType in GenericTypes)
                {
                    sb.Append(", ");
                    sb.Append(genericType.BaseType);
                    sb.Append(", ");
                    sb.Append(genericType.ToString());
                    sb.Append(", ");
                    sb.Append(genericType.ToSpecial(true));
                    sb.Append(", ");
                    sb.Append(genericType.CastToManaged ? $"_t{id}->ToManaged()" : $"_t{id}");

                    id++;
                }
            }

            sb.Append(")");

            return sb.ToString();
        }

        public string ToString(bool isReturn = false, bool allowConst = true, bool allowRef = true)
        {
            if (isReturn && ByPtr && !ByRef && !IsConst)
                return "MonoObject*";

            return $"{(IsConst && allowConst ? "const " : "")}{BaseType}{(ByRef && allowRef ? "&" : "")}{(ByPtr ? "*" : "")}";
        }

        public bool Equals(TypeDescription other)
        {
            if (other == null)
                return false;

            return string.Equals(BaseType, other.BaseType) && IsConst == other.IsConst && ByRef == other.ByRef && ByPtr == other.ByPtr;
        }

        public override bool Equals(object obj)
        {
            return obj is TypeDescription other && Equals(other);
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = (BaseType != null ? BaseType.GetHashCode() : 0);
                hashCode = (hashCode * 397) ^ IsConst.GetHashCode();
                hashCode = (hashCode * 397) ^ ByRef.GetHashCode();
                hashCode = (hashCode * 397) ^ ByPtr.GetHashCode();
                return hashCode;
            }
        }
    }
}
