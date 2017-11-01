// ReCrafted © 2016-2017 Always Too Late

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
