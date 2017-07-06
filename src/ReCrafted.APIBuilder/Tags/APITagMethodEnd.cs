// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagMethodEnd : APITag
    {
        public override void Process(string token, string[] parameters)
        {
            APITagMethod.Current = null;
            APITagComment.Current = null;
        }
    }
}
