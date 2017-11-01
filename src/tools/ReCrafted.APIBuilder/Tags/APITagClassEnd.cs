// ReCrafted © 2016-2017 Always Too Late

namespace ReCrafted.APIBuilder.Tags
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
