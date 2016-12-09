// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System.Diagnostics.CodeAnalysis;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    [SuppressMessage("ReSharper", "InconsistentNaming")]
    internal class OpenGLShader : Shader
    {
        protected override void Initialize(string shaderFile, ShaderMeta meta)
        {
            throw new System.NotImplementedException();
        }

        public override void SetValue<T>(string name, T value)
        {
            throw new System.NotImplementedException();
        }

        public override void SetValue<T>(int slot, T value)
        {
            throw new System.NotImplementedException();
        }

        public override void SetTexture(ShaderType type, int slot, Texture2D texture)
        {
            throw new System.NotImplementedException();
        }

        public override void SetRenderTexture(ShaderType type, int slot, RenderTarget texture)
        {
            throw new System.NotImplementedException();
        }

        public override void SetUnorderedAccessView(int slot, RenderTarget texture)
        {
            throw new System.NotImplementedException();
        }

        public override void UnsetUnorderedAccessView(int slot)
        {
            throw new System.NotImplementedException();
        }

        public override void UnsetRenderTexture(ShaderType type, int slot)
        {
            throw new System.NotImplementedException();
        }

        public override void UnsetTexture(ShaderType type, int slot)
        {
            throw new System.NotImplementedException();
        }

        public override void SetSampler(int slot, Sampler sampler)
        {
            throw new System.NotImplementedException();
        }

        public override void Apply()
        {
            throw new System.NotImplementedException();
        }

        public override void Draw(Mesh mesh)
        {
            throw new System.NotImplementedException();
        }

        public override void Dispose()
        {
            throw new System.NotImplementedException();
        }
    }
}
