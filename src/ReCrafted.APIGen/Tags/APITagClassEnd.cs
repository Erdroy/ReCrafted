// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted.APIGen.Tags
{
    public class APITagClassEnd : APITag
    {
        public override void Process(string token, string[] parameters)
        {
            APITagClass.Current = null;
            APITagComment.Current = null;
        }
    }
}
