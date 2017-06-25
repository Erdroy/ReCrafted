// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

using System;

namespace ReCrafted.APIGen.Tags
{
    public class APITagMethodCode : APITag
    {
        public override void Process(string token, string[] parameters)
        {
            if (parameters.Length != 1)
            {
                Console.WriteLine($"Invalid API_CODE token definition at line {APIBuilder.LineNumber}");
                return;
            }

            if (!APIBuilderUtils.IsString(parameters[0]))
            {
                Console.WriteLine($"Invalid API_CODE token parameter (code) definition at line {APIBuilder.LineNumber}");
                return;
            }

            APITagMethod.Current.CodeLines.Add(APIBuilderUtils.GetStringValue(parameters[0]));
        }
    }
}
