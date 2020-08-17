// ReCrafted (c) 2016-2020 Damian 'Erdroy' Korczowski. All rights reserved.

using System;
using System.Linq;
using ReCrafted.Tools.APIGenerator.Descriptions;
using ReCrafted.Tools.Common.CodeTokenizer;

namespace ReCrafted.Tools.APIGenerator
{
    public partial class Generator
    {
        private void HandleApiTag(Token token)
        {
            switch (token.Value)
            {
                case "API_CLASS_BODY":
                    _tokenizer.ExpectAllTokens(new[] {TokenType.LeftParent, TokenType.RightParent});
                    break;
                case "API_CLASS":
                    ParseClassTag();
                    break;
                case "API_FUNCTION":
                    _functions.Add(ParseFunctionTag());
                    break;
                case "API_PROPERTY":
                    ParsePropertyTag();
                    break;
                case "API_USING":
                    ParseUsingTag();
                    break;
                case "API_CUSTOM_TRANSLATION":
                    ParseCustomTranslationTag();
                    break;
            }
        }

        private void ParseCustomTranslationTag()
        {
            // API_CUSTOM_TRANSLATION(SomeNativeType_t, "SomeManagedType");

            _tokenizer.ExpectToken(TokenType.LeftParent);
            var nativeType = _tokenizer.ExpectToken(TokenType.Identifier);
            _tokenizer.ExpectToken(TokenType.Comma);
            var managedType = _tokenizer.ExpectToken(TokenType.String);
            _tokenizer.ExpectToken(TokenType.RightParent);

            // Add custom type translation
            CustomTranslation.Add(nativeType.Value, managedType.Value.Replace("\"", ""));
        }

        private void ParseUsingTag()
        {
            // API_USING("System.Collections.Generic")
            _tokenizer.ExpectToken(TokenType.LeftParent);
            var usingNamespaceToken = _tokenizer.ExpectToken(TokenType.String);
            _tokenizer.ExpectToken(TokenType.RightParent);

            // Cleanup using
            var usingNamespace = usingNamespaceToken.Value.Replace("\"", "");
            
            // TODO: Validate using

            Usings.Add(usingNamespace);
        }

        private void ParsePropertyTag()
        {
            var function = ParseFunctionTag();

            var propertyName = function.Name;
            if ((propertyName.StartsWith("Get") || propertyName.StartsWith("Set")) && function.Modifiers.Contains("noprefix"))
                propertyName = propertyName.Remove(0, 3);

            var property = _properties.Find(x => x.Name == propertyName);

            if (property != null)
            {
                if (property.Mode == PropertyMode.GetterSetter)
                    Console.WriteLine($"WARNING: Property with name '{propertyName}' has " +
                                      $"multiple functions declarations tagged with API_PROPERTY.");
                
                if(property.Access != function.Access)
                    Console.WriteLine(
                        $"WARNING: Property with name '{propertyName}' has multiple functions declarations with " +
                        $"different access modifier ({property.Access} / {function.Access}).");

                if (!property.Type.Equals(function.Parameters.FirstOrDefault().Type) && !property.Type.Equals(function.ReturnType))
                    Console.WriteLine(
                        $"WARNING: Property with name '{propertyName}' has multiple functions declarations with " +
                        $"different value types.");

                if (!function.ReturnType.IsVoid)
                    property.GetterFunctionName = function.Name;
                else
                    property.SetterFunctionName = function.Name;

                // Set the property to getter setter
                property.Mode = PropertyMode.GetterSetter;

                // Add missing modifiers
                foreach (var modifier in function.Modifiers)
                {
                    if (!property.Modifiers.Contains(modifier))
                    {
                        property.Modifiers.Add(modifier);
                    }
                }

                return;
            }

            property = new PropertyDescription
            {
                Name = propertyName,
                Type = function.ReturnType.IsVoid ? function.Parameters[0].Type : function.ReturnType,
                Access = function.Access,
                Comment = function.Comment,
                AllowMultithread = function.AllowMultithread,
                Mode = !function.ReturnType.IsVoid ? PropertyMode.Getter : PropertyMode.Setter,
                ForceByValue = function.ForceByValue
            };

            if (function.ReturnType.IsVoid)
                property.SetterFunctionName = function.Name;
            else
                property.GetterFunctionName = function.Name;

            // Add function modifiers to the property
            property.Modifiers.AddRange(function.Modifiers);

            _properties.Add(property);
        }

        private FunctionDescription ParseFunctionTag()
        {
            var desc = new FunctionDescription
            {
                // Comments (read the token before current one)
                Comment = ParseXmlCommentAbove()
            };

            // Parse tag arguments
            var tagArguments = ParseTagParameters();

            _tokenizer.SkipUntil(TokenType.Identifier);

            var token = _tokenizer.CurrentToken;

            // Skip FORCE_INLINE macro
            if (token.Value == "FORCE_INLINE" || token.Value == "FORCEINLINE")
            {
                desc.AggressiveInline = true;
                token = _tokenizer.NextToken();
            }

            var isStatic = token.Value == "static";

            if (isStatic)
                token = _tokenizer.NextToken();
            
            // Set virtual modifier
            var isVirtual = token.Value == "virtual";

            // If not virtual undo the token
            if (!isVirtual)
                _tokenizer.PreviousToken();

            desc.ReturnType = ParseNativeType();

            var functionName = _tokenizer.ExpectToken(TokenType.Identifier);

            // Set name
            desc.Name = functionName.Value;

            // Parse function parameters
            desc.Parameters.AddRange(ParseFunctionParameters());

            token = _tokenizer.NextToken();

            var isConst = token.Value == "const";

            if (isConst)
                token = _tokenizer.NextToken();

            var isOverride = token.Value == "override";

            for (var i = 0; i < tagArguments.Count; i++)
            {
                token = tagArguments[i];

                switch (token.Value)
                {
                    case "public":
                        desc.Access = AccessModifier.Public;
                        break;
                    case "protected":
                        desc.Access = AccessModifier.Protected;
                        break;
                    case "internal":
                        desc.Access = AccessModifier.Internal;
                        break;
                    case "private":
                        desc.Access = AccessModifier.Private;
                        break;
                    case "multithread":
                        desc.AllowMultithread = true;
                        break;
                    case "noproxy":
                    case "extern":
                        desc.Extern = true;
                        break;
                    case "byvalue":
                        desc.ForceByValue = true;
                        break;
                    case "noprefix":
                    case "static":
                    case "virtual":
                    case "override":
                    case "abstract":
                        desc.Modifiers.Add(token.Value);
                        break;
                    default:
                        Console.WriteLine(
                            $"WARNING: Unsupported modifier '{token.Value}' is passed to the API_FUNCTION tag of function {functionName.Value}!");
                        break;
                }
            }

            if (isVirtual && !desc.Modifiers.Contains("virtual"))
                desc.Modifiers.Add("virtual");

            if (isStatic && !desc.Modifiers.Contains("static"))
                desc.Modifiers.Add("static");

            if (isOverride && !desc.Modifiers.Contains("override"))
                desc.Modifiers.Add("override");

            return desc;
        }

        private void ParseClassTag()
        {
            var desc = new ClassDescription
            {
                // Comments (read the token before current one)
                Comment = ParseXmlCommentAbove()
            };

            // Parse tag arguments
            var tagArguments = ParseTagParameters();

            // Skip until `class` keyword.
            _tokenizer.SkipUntil(TokenType.Identifier);

            var keyword = _tokenizer.CurrentToken;

            if (keyword.Value == "template")
            {
                // Parse generics
                _tokenizer.ExpectToken(TokenType.LeftAngleBracket);

                while (true)
                {
                    _tokenizer.ExpectToken(TokenType.Identifier);
                    var genericType = _tokenizer.ExpectToken(TokenType.Identifier);

                    // Add generic type
                    desc.GenericTypes.Add(genericType.Value);

                    var nextToken = _tokenizer.NextToken();
                    if (nextToken.Type == TokenType.RightAngleBracket)
                        break;
                }

                // Skip until `class` keyword.
                _tokenizer.SkipUntil(TokenType.Identifier);
            }

            // Parse class name
            var className = _tokenizer.ExpectToken(TokenType.Identifier);

            // Set class name
            desc.Name = className.Value;

            // Parse namespace
            desc.Namespace = ParseNamespace(InputFile);

            // Parse class inheritance, support for single Type/Generic Type.
            var token = _tokenizer.NextToken();

            var isFinal = token.Value == "final";

            if (isFinal)
                token = _tokenizer.NextToken();

            if (token.Type == TokenType.Colon)
            {
                // Current class does have inheritance defined
                var accessToken = _tokenizer.ExpectToken(TokenType.Identifier);
                var access = accessToken.Value;

                Token nameToken;
                if (access != "public")
                {
                    Console.WriteLine($"WARNING: C++ class '{className.Value}' inherits a type with non-public access");

                    // Parsed access token is name token, when the 'public' modifier is missing.
                    // (Look at the first comment in this function)
                    nameToken = accessToken;
                }
                else
                {
                    nameToken = _tokenizer.ExpectToken(TokenType.Identifier);
                }

                token = _tokenizer.NextToken();

                if (token.Type == TokenType.LeftAngleBracket)
                {
                    // This is a generic type
                    var genericType = _tokenizer.ExpectToken(TokenType.Identifier);
                    _tokenizer.ExpectToken(TokenType.RightAngleBracket);

                    desc.Inherits = $"{nameToken.Value}<{genericType.Value}>";
                }
                else
                {
                    desc.Inherits = nameToken.Value;
                }
            }
            else if(desc.Name != "Object" && tagArguments.FindIndex(x => x.Value == "noinherit") == -1)
            {
                // Must inherit!
                throw new Exception($"Class {className.Value} must inherit Object or any other type that inherits Object!");
            }
            else
            {
                token = _tokenizer.PreviousToken();
            }

            for (var i = 0; i < tagArguments.Count; i++)
            {
                token = tagArguments[i];

                switch (token.Value)
                {
                    case "public":
                        desc.Access = AccessModifier.Public;
                        break;
                    case "protected":
                        desc.Access = AccessModifier.Protected;
                        break;
                    case "internal":
                        desc.Access = AccessModifier.Internal;
                        break;
                    case "private":
                        desc.Access = AccessModifier.Private;
                        break;
                    case "static":
                    case "sealed":
                    case "abstract":
                    case "partial":
                        desc.Modifiers.Add(token.Value);
                        break;
                    case "generic":
                        desc.CreateGenericFunctionality = true;
                        break;
                    case "noinherit":
                        // Skip class inheritance, just remove it, it will do it.
                        desc.Inherits = "";
                        break;
                    case "customNamespace":
                        if (tagArguments.Count == i + 1 || tagArguments[i + 1].Type != TokenType.String)
                            throw new Exception("Class custom namespace doesn't follow with string!");

                        // Override namespace with new one
                        // TODO: Check the namespace for invalid characters etc.
                        desc.Namespace = tagArguments[i + 1].Value.Replace("\"", "");
                        i++;
                        break;

                    default:
                        Console.WriteLine($"WARNING: Unknown class modifier '{token.Value}'.");
                        break;
                }
            }

            if(isFinal && !desc.Modifiers.Contains("sealed") && !desc.Modifiers.Contains("static"))
                desc.Modifiers.Add("sealed");

            _class = desc;
        }
    }
}
