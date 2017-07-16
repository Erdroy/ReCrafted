// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Linq;

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagProperty : APITag
    {
        public static APITagMethod Current;

        public string Name { get; set; }

        public string Type { get; set; }

        public string Comment { get; set; }

        public APIParameters Access { get; set; }

        public APIParameters Modifier { get; set; }

        public bool GetOnly { get; set; }

        public bool ByRef { get; set; }

        public APITagProperty()
        {
            APITagClass.Current.Properties.Add(this);

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
            GetOnly = parameters.Contains("GET") && !parameters.Contains("GETSET");
            ByRef = parameters.Contains("BY_REF");

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
                Console.WriteLine($"Invalid API_PROPERTY access modifier at line {APIBuilder.LineNumber}");
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
                Console.WriteLine($"Invalid API_PROPERTY modifier at line {APIBuilder.LineNumber}");
                return;
            }
            
            Type = APIBuilderUtils.GetStringValue(parameters[2]);
            Name = APIBuilderUtils.GetStringValue(parameters[3]);

        }
    }
}
