// ReCrafted (c) 2016-2018 Always Too Late

namespace ReCrafted.API.Content
{
    public partial class Asset
    {
        private bool _unloaded;

        public void Unload()
        {
            if (_unloaded)
                return;

            _unloaded = true;

            // Unload this asset
            ContentManagerInternals.UnloadAsset(NativePtr);
        }
    }
}
