// ReCrafted © 2016-2017 Always Too Late

using System;

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagUsing : APITag
    {
        public string Namespace { get; set; }

        public APITagUsing()
        {
            APITagFile.Current.Usings.Add(this);
        }

        public override void Process(string token, string[] parameters)
        {
            if (parameters.Length != 1 || !APIBuilderUtils.IsString(parameters[0]))
            {
                Console.WriteLine($"Invalid API_USING token definition at line {APIBuilder.LineNumber}");
                return;
            }

            var name = APIBuilderUtils.GetStringValue(parameters[0]);

            if (string.IsNullOrEmpty(name))
            {
                Console.WriteLine($"Invalid API_USING namespace at line {APIBuilder.LineNumber}");
                return;
            }

            Namespace = name;
        }
    }
}
