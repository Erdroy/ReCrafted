// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    /// <summary>
    /// OpenGLShader class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    internal sealed class OpenGLShader : Shader
    {
        /// <summary>
        /// Initializes the shader.
        /// </summary>
        /// <param name="shaderFile">Shader file name.</param>
        /// <param name="meta">The shader meta.</param>
        protected override void Initialize(string shaderFile, ShaderMeta meta)
        {
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue<T>(string name, T value) 
        {
        }


        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue<T>(string name, T[] value)
        {
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="slot">The field slot.</param>
        /// <param name="value">The value.</param>
        public override void SetValue<T>(int slot, T value)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Set texture at slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public override void SetTexture(ShaderType type, int slot, Texture2D texture)
        {
        }

        /// <summary>
        /// Set renderTarget at slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public override void SetRenderTexture(ShaderType type, int slot, RenderTarget texture)
        {
        }

        /// <summary>
        /// Set UAV of renderTarget at slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public override void SetUnorderedAccessView(int slot, RenderTarget texture)
        {
        }

        /// <summary>
        /// Unset UAV of renderTarget from slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        public override void UnsetUnorderedAccessView(int slot)
        {
        }

        /// <summary>
        /// Unset renderTarget from slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        public override void UnsetRenderTexture(ShaderType type, int slot)
        {
        }

        /// <summary>
        /// Unset texture from slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        public override void UnsetTexture(ShaderType type, int slot)
        {
        }

        /// <summary>
        /// Set sampler at slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        /// <param name="sampler">The sampler.</param>
        public override void SetSampler(int slot, Sampler sampler)
        {
        }

        /// <summary>
        /// Apply the shader.
        /// </summary>
        public override void Apply()
        {
        }

        /// <summary>
        /// Apply values changes.
        /// </summary>
        public override void ApplyChanges()
        {
        }

        /// <summary>
        /// Draws mesh using this shader.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        public override void Draw(Mesh mesh)
        {
        }

        /// <summary>
        /// Disposes the shader.
        /// </summary>
        public override void Dispose()
        {
        }

        // private
        private static string ParseShaderSource(string sourceCode, string file)
        {
            return "";
        }
    }
}