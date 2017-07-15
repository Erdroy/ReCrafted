// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;
using System.Linq;

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagMethod : APITag
    {
        public static APITagMethod Current;

        public string Name { get; set; }

        public string Comment { get; set; }

        public string ReturnType { get; set; }

        public APIParameters Access { get; set; }

        public APIParameters Modifier { get; set; }

        public bool IsExtern { get; set; }

        public List<APITagParam> Parameters = new List<APITagParam>();

        public List<string> CodeLines = new List<string>();

        public APITagMethod()
        {
            Current = this;
            APITagClass.Current.Methods.Add(this);

            if (APITagComment.Current != null)
                Comment = APITagComment.Current.Text;
            APITagComment.Current = null;
        }

        public string GetAccess()
        {
            switch (Access)
            {
                case APIParameters.Public:
                    return "public";
                case APIParameters.Internal:
                    return "internal";
                case APIParameters.Private:
                    return "private";
                case APIParameters.Protected:
                    return "protected";
                default:
                    return "public";
            }
        }

        public string GetModifier()
        {
            switch (Modifier)
            {
                case APIParameters.Abstract:
                    return "abstract ";
                case APIParameters.Virtual:
                    return "virtual ";
                case APIParameters.Override:
                    return "override ";
                case APIParameters.Static:
                    return "static ";
                default:
                    return "";
            }
        }

        public override void Process(string token, string[] parameters)
        {
            // API_METHOD(PUBLIC, STATIC, "Write");

            IsExtern = parameters.Contains("EXTERN");

            if (parameters.Length != 3 && parameters.Length != 4)
            {
                Console.WriteLine($"Invalid API_METHOD token definition at line {APIBuilder.LineNumber}");
                return;
            }

            if (APIBuilderUtils.IsString(parameters[0]) ||
                APIBuilderUtils.IsString(parameters[1]) ||
                !APIBuilderUtils.IsString(parameters[2])
                )
            {
                Console.WriteLine($"Invalid API_METHOD token parameters definition at line {APIBuilder.LineNumber}");
                return;
            }

            if (parameters[0] == "PUBLIC")
                Access = APIParameters.Public;
            else if (parameters[0] == "INTERNAL")
                Access = APIParameters.Internal;
            else if (parameters[0] == "PRIVATE")
                Access = APIParameters.Private;
            else if (parameters[0] == "PROTECTED")
                Access = APIParameters.Protected;
            else
            {
                Console.WriteLine($"Invalid API_METHOD access modifier at line {APIBuilder.LineNumber}");
                return;
            }

            if (parameters[1] == "REGULAR")
                Modifier = APIParameters.Regular;
            else if (parameters[1] == "ABSTRACT")
                Modifier = APIParameters.Abstract;
            else if (parameters[1] == "OVERRIDE")
                Modifier = APIParameters.Override;
            else if (parameters[1] == "VIRTUAL")
                Modifier = APIParameters.Virtual;
            else if (parameters[1] == "STATIC")
                Modifier = APIParameters.Static;
            else
            {
                Console.WriteLine($"Invalid API_METHOD modifier at line {APIBuilder.LineNumber}");
                return;
            }

            var name = APIBuilderUtils.GetStringValue(parameters[2]);

            if (string.IsNullOrEmpty(name))
            {
                Console.WriteLine($"Invalid API_METHOD method name at line {APIBuilder.LineNumber}");
                return;
            }

            Name = name;
        }
    }
}
