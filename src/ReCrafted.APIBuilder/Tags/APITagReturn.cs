// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagReturn : APITag
    {
        public override void Process(string token, string[] parameters)
        {
            if (parameters.Length != 1)
            {
                Console.WriteLine($"Invalid API_RETURN token definition at line {APIBuilder.LineNumber}");
                return;
            }

            if (!APIBuilderUtils.IsString(parameters[0]))
            {
                Console.WriteLine($"Invalid API_RETURN token parameter (text) definition at line {APIBuilder.LineNumber}");
                return;
            }

            APITagMethod.Current.ReturnType = APIBuilderUtils.GetStringValue(parameters[0]);
        }
    }
}
