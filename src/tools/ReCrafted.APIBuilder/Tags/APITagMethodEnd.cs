// ReCrafted (c) 2016-2018 Always Too Late

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
