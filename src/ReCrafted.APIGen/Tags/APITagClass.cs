// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;
using System.Linq;

namespace ReCrafted.APIGen.Tags
{
    public class APITagClass : APITag
    {
        public static APITagClass Current;
        
        public string Name { get; set; }

        public string BaseName { get; set; }

        public string Namespace { get; set; }

        public string Comment { get; set; }

        public APIParameters Access { get; set; }

        public APIParameters Modifier { get; set; }

        public bool IsPartial { get; set; }

        public List<APITagMethod> Methods = new List<APITagMethod>();

        public APITagClass()
        {
            Current = this;
            APITagFile.Current.Classes.Add(this);

            if(APITagComment.Current != null)
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
                case APIParameters.Sealed:
                    return "sealed ";
                case APIParameters.Static:
                    return "static ";
                default:
                    return "";
            }
        }

        public override void Process(string token, string[] parameters)
        {
            // API_CLASS(PUBLIC, REGULAR, "ReCrafted.API", "Logger");
            // API_CLASS_INHERIT(PUBLIC, REGULAR, "ReCrafted.API", "Logger", "ILogger");

            IsPartial = parameters.Contains("PARTIAL");

            if (parameters.Length < 4 || parameters.Length > 6)
            {
                Console.WriteLine($"Invalid API_CLASS token definition at line {APIBuilder.LineNumber}");
                return;
            }

            if (APIBuilderUtils.IsString(parameters[0]) || 
                APIBuilderUtils.IsString(parameters[1]) ||
                !APIBuilderUtils.IsString(parameters[2]) ||
                !APIBuilderUtils.IsString(parameters[3])
                )
            {
                Console.WriteLine($"Invalid API_CLASS token parameters definition at line {APIBuilder.LineNumber}");
                return;
            }

            if(parameters[0] == "PUBLIC")
                Access = APIParameters.Public;
            else if (parameters[0] == "INTERNAL")
                Access = APIParameters.Internal;
            else if (parameters[0] == "PRIVATE")
                Access = APIParameters.Private;
            else
            {
                Console.WriteLine($"Invalid API_CLASS access modifier at line {APIBuilder.LineNumber}");
                return;
            }

            if (parameters[1] == "REGULAR")
                Modifier = APIParameters.Regular;
            else if (parameters[1] == "ABSTRACT")
                Modifier = APIParameters.Abstract;
            else if (parameters[1] == "SEALED")
                Modifier = APIParameters.Sealed;
            else if (parameters[1] == "STATIC")
                Modifier = APIParameters.Static;
            else
            {
                Console.WriteLine($"Invalid API_CLASS modifier at line {APIBuilder.LineNumber}");
                return;
            }

            var ns = APIBuilderUtils.GetStringValue(parameters[2]);

            if(string.IsNullOrEmpty(ns))
            {
                Console.WriteLine($"Invalid API_CLASS class namespace at line {APIBuilder.LineNumber}");
                return;
            }

            Namespace = ns;

            var name = APIBuilderUtils.GetStringValue(parameters[3]);

            if (string.IsNullOrEmpty(name))
            {
                Console.WriteLine($"Invalid API_CLASS class name at line {APIBuilder.LineNumber}");
                return;
            }

            Name = name;

            if (parameters.Length >= 5)
            {
                var basename = APIBuilderUtils.GetStringValue(parameters[4]);

                if (string.IsNullOrEmpty(basename))
                {
                    Console.WriteLine($"Invalid API_CLASS base class name at line {APIBuilder.LineNumber}");
                    return;
                }

                BaseName = basename;
            }
        }
    }
}
