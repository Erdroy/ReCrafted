// ReCrafted Editor (c) 2016-2018 Always Too Late

using ReCrafted.Editor.Content.Assets;

namespace ReCrafted.Editor.Content.Previews
{
    public interface IPreviewGenerator
    {
        void Initialize();
        void Shutdown();

        TextureAsset Generate(Asset asset);
    }
}
