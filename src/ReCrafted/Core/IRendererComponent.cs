namespace ReCrafted.Core
{
    public interface IRendererComponent
    {
        void Init();
        void Tick();
        void Draw();
    }
}