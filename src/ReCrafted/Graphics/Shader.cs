// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.IO;
using ReCrafted.Graphics.Renderers.D3D11;
using ReCrafted.Graphics.Renderers.OpenGL;
using ReCrafted.Utilities;
using SharpDX;

namespace ReCrafted.Graphics
{
    /// <summary>
    /// Shader class.
    /// </summary>
    public abstract class Shader : IDisposable
    {
        private static readonly Dictionary<string, Shader> HotReload = new Dictionary<string, Shader>();
        private static FileSystemWatcher _fileWatcher;
        private static bool _hotReloadInitialized;

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
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, Matrix value);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, Vector2 value);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, Vector3 value);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, Vector4 value);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, short value);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, int value);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, ushort value);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, uint value);

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public abstract void SetValue(string name, bool value);

        /// <summary>
        /// Set texture at slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public abstract void SetTexture(ShaderType type, int slot, Texture2D texture);

        /// <summary>
        /// Set renderTarget at slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public abstract void SetRenderTexture(ShaderType type, int slot, RenderTarget texture);

        /// <summary>
        /// Set UAV of renderTarget at slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public abstract void SetUnorderedAccessView(int slot, RenderTarget texture);

        /// <summary>
        /// Unset UAV of renderTarget from slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        public abstract void UnsetUnorderedAccessView(int slot);

        /// <summary>
        /// Unset renderTarget from slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        public abstract void UnsetRenderTexture(ShaderType type, int slot);

        /// <summary>
        /// Unset texture from slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        public abstract void UnsetTexture(ShaderType type, int slot);

        /// <summary>
        /// Set sampler at slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        /// <param name="sampler">The sampler.</param>
        public abstract void SetSampler(int slot, Sampler sampler);

        /// <summary>
        /// Apply the shader
        /// </summary>
        public abstract void Apply();

        /// <summary>
        /// Apply value changes.
        /// </summary>
        public abstract void ApplyChanges();

        /// <summary>
        /// Draws mesh using this shader.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        public abstract void Draw(Mesh mesh);

        /// <summary>
        /// Draw using fullscreen quad generated by GPU.
        /// </summary>
        public abstract void DrawFullscreen();
        
        /// <summary>
        /// Disposes the shader.
        /// </summary>
        public abstract void Dispose();

        /// <summary>
        /// Loads shader from file.
        /// </summary>
        /// <param name="shaderName">The shader name.</param>
        /// <param name="hotReload">Use filesystem watch to recompile the shader when changed.</param>
        /// <returns>The loaded shader.</returns>
        public static Shader FromFile(string shaderName, bool hotReload = true)
        {
            var shaderSourceFile = "assets/shaders/" + shaderName;
            var shaderMetaFile = "assets/shaders/" + shaderName + ".json";

            var shaderMeta = ShaderMeta.FromFile(shaderMetaFile);

            if (hotReload)
            {
                if (!_hotReloadInitialized)
                {
                    InitializeHotReload();
                    _hotReloadInitialized = true;
                }
            }

            switch (Renderer.RendererApi)
            {
                case RendererApi.D3D11:
                    {
                        var shader = new D3D11Shader();

                        if (hotReload)
                            HotReload.Add(shaderName, shader);

                        shader.Initialize(shaderSourceFile, shaderMeta);
                        return shader;
                    }
                case RendererApi.OpenGL:
                    {
                        var shader = new OpenGLShader();
                        shader.Initialize(shaderSourceFile, shaderMeta);
                        return shader;
                    }
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

        private static void InitializeHotReload()
        {
            _fileWatcher = new FileSystemWatcher
            {
                Path = "assets/shaders",
                Filter = "*.*",
                EnableRaisingEvents = true,
                IncludeSubdirectories = true,
                NotifyFilter = NotifyFilters.LastWrite | NotifyFilters.CreationTime | NotifyFilters.FileName
            };

            _fileWatcher.Changed += ShaderFileChanged;
        }

        private static void ShaderFileChanged(object sender, FileSystemEventArgs e)
        {
            // reload shader
            var name = e.Name.Replace(".json", "").Replace(".hlsl", "").Replace("\\", "/");

            if (!HotReload.ContainsKey(name))
                return;

            var shader = HotReload[name];
            shader.Initialize(e.FullPath, ShaderMeta.FromFile(e.FullPath.Replace("hlsl", "json")));
        }
    }
}