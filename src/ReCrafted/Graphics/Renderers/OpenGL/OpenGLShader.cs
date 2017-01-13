// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.IO;
using OpenTK.Graphics.OpenGL;
using ReCrafted.Utilities;
using SharpDX;

namespace ReCrafted.Graphics.Renderers.OpenGL
{
    /// <summary>
    /// OpenGLShader class.
    /// </summary>
    // ReSharper disable once InconsistentNaming
    internal sealed class OpenGLShader : Shader
    {
        private int _bufferLocation = -1;
        private int _buffer = -1;

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

            // load uniform block buffer if needed
            if (Meta.ConstantBuffers.Length == 1)
            {
                // find block location
                _bufferLocation = GL.GetUniformBlockIndex(ShaderProgram, Meta.ConstantBuffers[0].BufferName);

                // get buffer info
                var bufferSize = Meta.ConstantBuffers[0].Size; // get the size of the buffer
                var size = (IntPtr) bufferSize;

                // create temporary 0-filled data buffer
                var data = new byte[bufferSize];
                _buffer = GL.GenBuffer();
                GL.BindBuffer(BufferTarget.UniformBuffer, _buffer);
                GL.BufferData(BufferTarget.UniformBuffer, size, data, BufferUsageHint.DynamicDraw);

                // clear
                GL.BindBuffer(BufferTarget.UniformBuffer, 0);
            }
            
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, Matrix value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)Matrix.SizeInBytes, ref value);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, Vector2 value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)Vector2.SizeInBytes, ref value);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, Vector3 value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)Vector3.SizeInBytes, ref value);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, Vector4 value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)Vector4.SizeInBytes, ref value);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, short value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)sizeof(short), ref value);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, int value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)sizeof(int), ref value);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, ushort value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)sizeof(ushort), ref value);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, uint value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)sizeof(uint), ref value);
            OpenGLRenderer.CheckError();
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue(string name, bool value)
        {
            var offset = Meta.ConstantBuffers[0].GetOffset(name);
            GL.BufferSubData(BufferTarget.UniformBuffer, (IntPtr)offset, (IntPtr)sizeof(bool), ref value);
            OpenGLRenderer.CheckError();
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

            var variable = Meta.Variables[slot];

            var glRt = (OpenGLRenderTarget)texture;

            var loc = GL.GetUniformLocation(ShaderProgram, variable.Name);
            GL.Uniform1(loc, 0);

            GL.ActiveTexture(TextureUnit.Texture0);
            GL.BindTexture(TextureTarget.Texture2D, glRt.Texture);

            OpenGLRenderer.CheckError();
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

            // bind uniform block buffer is needed
            if (_bufferLocation >= 0)
            {
                GL.UniformBlockBinding(ShaderProgram, _bufferLocation, 0);
                GL.BindBufferBase(BufferRangeTarget.UniformBuffer, 0, _buffer);
            }

            OpenGLRenderer.CheckError();
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

            if(_buffer >= 0)
                GL.DeleteBuffer(_buffer);

            OpenGLRenderer.CheckError();
        }

        public ShaderMeta Meta { get; private set; }

        public int VertexShader { get; private set; } = -1;

        public int FragmentShader { get; private set; } = -1;

        public int ShaderProgram { get; private set; } = -1;
    }
}