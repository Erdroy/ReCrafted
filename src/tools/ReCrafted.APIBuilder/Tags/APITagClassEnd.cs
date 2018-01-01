// ReCrafted (c) 2016-2018 Always Too Late

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
