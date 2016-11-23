
using System;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Utilities;
using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Shader class.
    /// </summary>
    public abstract class Shader : IDisposable
    {
        // lock from creating
        protected Shader() { }

        /// <summary>
        /// Initializes the shader.
        /// </summary>
        /// <param name="shaderFile">Shader file name.</param>
        /// <param name="meta">The shader meta.</param>
        protected abstract void Initialize(string shaderFile, ShaderMeta meta);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue<T>(string name, T value) where T : struct;

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="slot">The field slot.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue<T>(int slot, T value) where T : struct;

        /// <summary>
        /// Draws mesh using this shader.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        public abstract void Draw(Mesh mesh);

        /// <summary>
        /// Disposes the shader.
        /// </summary>
        public abstract void Dispose();

        /// <summary>
        /// Loads shader from file.
        /// </summary>
        /// <param name="shaderName">The shader name.</param>
        /// <returns>The loaded shader.</returns>
        public static Shader FromFile(string shaderName)
        {
            var shaderSourceFile = "assets/shaders/" + shaderName + ".hlsl";
            var shaderMetaFile = "assets/shaders/" + shaderName + ".json";

            var shaderMeta = ShaderMeta.FromFile(shaderMetaFile);

            if (Renderer.RendererApi == RendererApi.D3D11)
            {
                var d3D11Shader = new D3D11Shader();
                d3D11Shader.Initialize(shaderSourceFile, shaderMeta);
                return d3D11Shader;
            }

            // TODO: Implement renderers

            throw new ReCraftedException("Invalid RendererApi!");
        }

        /// <summary>
        /// Converts type name to type size.
        /// </summary>
        /// <param name="type">The type name.</param>
        /// <returns>The size, -1 when not recognized.</returns>
        public static int GetTypeSize(string type)
        {
            switch (type)
            {
                // TODO: Add more types

                case "bool":
                    return 1;

                case "half":
                    return 1;

                case "uint":
                case "sint":
                case "int":
                case "float":
                    return 4;

                case "double":
                case "float2":
                    return 8;

                case "float3":
                    return 12;

                case "float4":
                    return 16;

                case "float3x3":
                    return 36;

                case "float3x4":
                case "float4x3":
                    return 48;

                case "float4x4":
                case "matrix":
                    return Matrix.SizeInBytes;
            }

            return -1;
        }
    }
}