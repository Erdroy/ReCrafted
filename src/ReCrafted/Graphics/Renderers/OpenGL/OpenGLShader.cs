// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.IO;
using OpenTK.Graphics.OpenGL;
using ReCrafted.Utilities;

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
            int statusCode;
            string info;

            var vs = shaderFile + ".vert.glsl";
            var fs = shaderFile + ".frag.glsl";

            if (!File.Exists(vs) || !File.Exists(fs))
                return;

            Meta = meta;

            VertexShader = GL.CreateShader(OpenTK.Graphics.OpenGL.ShaderType.VertexShader);
            FragmentShader = GL.CreateShader(OpenTK.Graphics.OpenGL.ShaderType.FragmentShader);

            var vssource = File.ReadAllText(vs);
            var fssource = File.ReadAllText(fs);

            GL.ShaderSource(VertexShader, vssource);
            GL.ShaderSource(FragmentShader, fssource);

            // compile vertex shader
            GL.CompileShader(VertexShader);

            GL.GetShaderInfoLog(VertexShader, out info);
            GL.GetShader(VertexShader, ShaderParameter.CompileStatus, out statusCode);

            if (statusCode != 1)
                throw new ReCraftedException(info);

            // compile fragment shader
            GL.CompileShader(FragmentShader);

            GL.GetShaderInfoLog(FragmentShader, out info);
            GL.GetShader(FragmentShader, ShaderParameter.CompileStatus, out statusCode);

            if (statusCode != 1)
                throw new ReCraftedException(info);

            ShaderProgram = GL.CreateProgram();
            GL.AttachShader(ShaderProgram, VertexShader);
            GL.AttachShader(ShaderProgram, FragmentShader);
            GL.LinkProgram(ShaderProgram);
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
            // http://stackoverflow.com/questions/7357626/framebuffer-and-using-shaders-in-opengl

            var glRt = (OpenGLRenderTarget)texture;
            
            GL.BindTexture(TextureTarget.Texture2D, glRt.Texture);
            GL.ActiveTexture(TextureUnit.Texture0);

            var loc = GL.GetUniformLocation(ShaderProgram, "m_texture");
            GL.Uniform1(loc, 1);
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
            // not used
        }

        /// <summary>
        /// Apply the shader.
        /// </summary>
        public override void Apply()
        {
            GL.UseProgram(ShaderProgram);
        }

        /// <summary>
        /// Apply values changes.
        /// </summary>
        public override void ApplyChanges()
        {
            // not used
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
            if(ShaderProgram >= -1)
                GL.DeleteProgram(ShaderProgram);

            if(VertexShader >= 0)
                GL.DeleteShader(VertexShader);

            if (FragmentShader >= 0)
                GL.DeleteShader(FragmentShader);
        }

        public ShaderMeta Meta { get; private set; }

        public int VertexShader { get; private set; } = -1;

        public int FragmentShader { get; private set; } = -1;

        public int ShaderProgram { get; private set; } = -1;
    }
}