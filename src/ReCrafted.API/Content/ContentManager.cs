// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Content
{
    public static class ContentManager
    {
        public static TAsset LoadAsset<TAsset>(string assetFile) where TAsset : Asset
        {
            return (TAsset)LoadAsset(assetFile);
        }

        public static Asset LoadAsset(string assetFile)
        {
            return null;
        }
    }
}
