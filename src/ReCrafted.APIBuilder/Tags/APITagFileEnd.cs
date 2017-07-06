// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski and Mateusz 'Maturas' Zawistowski

namespace ReCrafted.APIBuilder.Tags
{
    public class APITagFileEnd : APITag
    {
        public override void Process(string token, string[] parameters)
        {
            APITagFile.Current = null;
            // we're done.
        }
    }
}
