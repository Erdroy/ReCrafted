// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;
using System.Collections.Generic;

namespace ReCrafted.APIGen.Tags
{
    public class APITagClass : APITag
    {
        public APITagFile Owner { get; set; }

        public string Name { get; set; }

        public string BaseName { get; set; }

        public string Namespace { get; set; }

        public APIParameters Access { get; set; }

        public APIParameters Modifier { get; set; }

        public List<APITagMethod> Methods = new List<APITagMethod>();

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
            switch (Access)
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
    }
}
