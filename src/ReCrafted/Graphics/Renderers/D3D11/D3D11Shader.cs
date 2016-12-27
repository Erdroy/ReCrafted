// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
using System.IO;
using System.Windows.Forms;
using ReCrafted.Utilities;
using SharpDX;
using SharpDX.D3DCompiler;
using SharpDX.Direct3D;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using Buffer = SharpDX.Direct3D11.Buffer;
using MapFlags = SharpDX.Direct3D11.MapFlags;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    /// <summary>
    /// D3D11Shader class.
    /// </summary>
    internal sealed class D3D11Shader : Shader
    {
        // todos
        // TODO: Multi-pass support
        // TODO: Multi-constant buffer support
        
        private ShaderMeta _meta;

        internal VertexShader VertexShader;
        internal PixelShader PixelShader;
        internal ComputeShader ComputeShader;

        internal InputLayout InputLayout;

        private Buffer _constantBuffer;
        private DataBuffer _backingBuffer;
        private IntPtr _dataPointer;
        private bool _isDirty;

        /// <summary>
        /// Initializes the shader.
        /// </summary>
        /// <param name="shaderFile">Shader file name.</param>
        /// <param name="meta">The shader meta.</param>
        protected override void Initialize(string shaderFile, ShaderMeta meta)
        {
            // allow to hot-reload
            Dispose();

            if(meta.Passes.Length == 0)
                throw new ReCraftedException("Cannot load shader! There is no any pass in this shader.");

            if (meta.Passes.Length > 1)
                throw new ReCraftedException("Multi-pass shaders are not supported, yet.");

            var pass = meta.Passes[0];

            if(pass.Profile != "3_0" && pass.Profile != "4_0" && pass.Profile != "5_0")
                throw new ReCraftedException($"Shader model {pass.Profile} is not supported.");
            
            var hasVs = pass.VertexShader != string.Empty;
            var hasPs = pass.PixelShader != string.Empty;
            var hasCs = pass.ComputeShader != string.Empty;

            var hasConstBuffer = meta.ConstantBuffers.Length == 1;

            if (meta.ConstantBuffers.Length > 1)
                throw new ReCraftedException("Multi-constant buffers are not supported, yet.");
            
            var device = D3D11Renderer.GetDevice();

            // parse source code
            var sourceCode = ParseShaderSource(File.ReadAllText(shaderFile), shaderFile);

            if (hasVs)
            {
                // compile vertex shader
                var bytecode = ShaderBytecode.Compile(sourceCode, pass.VertexShader, "vs_" + pass.Profile);

                if (bytecode.Bytecode == null) // error
                    MessageBox.Show(@"VShader load error: " + bytecode.Message);

                VertexShader = new VertexShader(device, bytecode);

                // build inputLayout inputElements
                var inputElements = new List<InputElement>();
               
                foreach (var inputElement in pass.VertexLayoutInputs)
                {
                    inputElements.Add(new InputElement(inputElement.InputSignature, 0, D3D11Format.FromString(inputElement.InputType), 0));
                }

                // create inputLayout
                InputLayout = new InputLayout(device, bytecode, inputElements.ToArray());
            }

            if (hasPs)
            {
                // compile pixel shader
                var bytecode = ShaderBytecode.Compile(sourceCode, pass.PixelShader, "ps_" + pass.Profile);

                if (bytecode.Bytecode == null) // error
                    MessageBox.Show(@"PShader load error: " + bytecode.Message);

                PixelShader = new PixelShader(device, bytecode);
            }

            if (hasCs)
            {
                // compile compute shader
                var bytecode = ShaderBytecode.Compile(sourceCode, pass.ComputeShader, "cs_" + pass.Profile);

                if (bytecode.Bytecode == null) // error
                    MessageBox.Show(@"CShader load error: " + bytecode.Message);

                ComputeShader = new ComputeShader(device, bytecode);
            }

            if (hasConstBuffer)
            {
                var cbuffer = meta.ConstantBuffers[0];
                var size = (int) ReCraftedMath.NearestRound(cbuffer.Size, 16);

                _backingBuffer = new DataBuffer(size);
                _dataPointer = _backingBuffer.DataPointer;
                _constantBuffer = Buffer.Create(device, BindFlags.ConstantBuffer, ref _dataPointer, size, ResourceUsage.Dynamic, CpuAccessFlags.Write);
            }

            _meta = meta;
        }

        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue<T>(string name, T value) 
        {
            var offset = _meta.ConstantBuffers[0].GetOffset(name);
            _backingBuffer.Set(offset, value);

            _isDirty = true;
        }


        /// <summary>
        /// Sets value in default constant buffer.
        /// </summary>
        /// <typeparam name="T">The value type.</typeparam>
        /// <param name="name">The name.</param>
        /// <param name="value">The value.</param>
        public override void SetValue<T>(string name, T[] value)
        {
            var offset = _meta.ConstantBuffers[0].GetOffset(name);
            _backingBuffer.Set(offset, value);

            _isDirty = true;
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
            var deviceContext = D3D11Renderer.GetDeviceContext();
            switch (type)
            {
                case ShaderType.PS:
                    deviceContext.PixelShader.SetShaderResource(slot, ((D3D11Texture2D)texture).ResourceView);
                    break;
                case ShaderType.VS:
                    deviceContext.VertexShader.SetShaderResource(slot, ((D3D11Texture2D)texture).ResourceView);
                    break;
                case ShaderType.CS:
                    deviceContext.ComputeShader.SetShaderResource(slot, ((D3D11Texture2D)texture).ResourceView);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(type), type, null);
            }
        }

        /// <summary>
        /// Set renderTarget at slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public override void SetRenderTexture(ShaderType type, int slot, RenderTarget texture)
        {
            var deviceContext = D3D11Renderer.GetDeviceContext();
            var res = (D3D11RenderTarget)texture;
            switch (type)
            {
                case ShaderType.PS:
                    deviceContext.PixelShader.SetShaderResource(slot, res.ResourceView);
                    break;
                case ShaderType.VS:
                    deviceContext.VertexShader.SetShaderResource(slot, res.ResourceView);
                    break;
                case ShaderType.CS:
                    deviceContext.ComputeShader.SetShaderResource(slot, res.ResourceView);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(type), type, null);
            }
        }

        /// <summary>
        /// Set UAV of renderTarget at slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public override void SetUnorderedAccessView(int slot, RenderTarget texture)
        {
            var deviceContext = D3D11Renderer.GetDeviceContext();
            var res = (D3D11RenderTarget)texture;
            deviceContext.ComputeShader.SetUnorderedAccessView(slot, res.UnorderedAccessView);
        }

        /// <summary>
        /// Unset UAV of renderTarget from slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        public override void UnsetUnorderedAccessView(int slot)
        {
            var deviceContext = D3D11Renderer.GetDeviceContext();
            deviceContext.ComputeShader.SetUnorderedAccessView(slot, null);
        }

        /// <summary>
        /// Unset renderTarget from slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        public override void UnsetRenderTexture(ShaderType type, int slot)
        {
            UnsetTexture(type, slot);
        }

        /// <summary>
        /// Unset texture from slot.
        /// </summary>
        /// <param name="type">The shader type.</param>
        /// <param name="slot">The slot.</param>
        public override void UnsetTexture(ShaderType type, int slot)
        {
            var deviceContext = D3D11Renderer.GetDeviceContext();
            switch (type)
            {
                case ShaderType.PS:
                    deviceContext.PixelShader.SetShaderResource(slot, null);
                    break;
                case ShaderType.VS:
                    deviceContext.VertexShader.SetShaderResource(slot, null);
                    break;
                case ShaderType.CS:
                    deviceContext.ComputeShader.SetShaderResource(slot, null);
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(type), type, null);
            }
        }

        /// <summary>
        /// Set sampler at slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        /// <param name="sampler">The sampler.</param>
        public override void SetSampler(int slot, Sampler sampler)
        {
            var deviceContext = D3D11Renderer.GetDeviceContext();
            deviceContext.PixelShader.SetSampler(slot, ((D3D11Sampler)sampler).SamplerState);
        }

        /// <summary>
        /// Apply the shader.
        /// </summary>
        public override void Apply()
        {
            var deviceContext = D3D11Renderer.GetDeviceContext();

            if (VertexShader != null)
                deviceContext.VertexShader.Set(VertexShader);

            if (PixelShader != null)
                deviceContext.PixelShader.Set(PixelShader);

            if (ComputeShader != null)
                deviceContext.ComputeShader.Set(ComputeShader);

            if (InputLayout != null)
                deviceContext.InputAssembler.InputLayout = InputLayout;

            // apply constant buffer
            deviceContext.ComputeShader.SetConstantBuffer(0, _constantBuffer);
            deviceContext.VertexShader.SetConstantBuffer(0, _constantBuffer);
            deviceContext.PixelShader.SetConstantBuffer(0, _constantBuffer);
        }

        /// <summary>
        /// Apply values changes.
        /// </summary>
        public override void ApplyChanges()
        {
            // update constant buffer if needed
            if (!_isDirty)
                return;

            var deviceContext = D3D11Renderer.GetDeviceContext();

            DataStream stream;
            deviceContext.MapSubresource(_constantBuffer, MapMode.WriteDiscard, MapFlags.None, out stream);

            // update
            stream.Position = 0;
            stream.Write(_backingBuffer.DataPointer, 0, _backingBuffer.Size);

            stream.Dispose();
            deviceContext.UnmapSubresource(_constantBuffer, 0);

            _isDirty = false;
        }

        /// <summary>
        /// Draws mesh using this shader.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        public override void Draw(Mesh mesh)
        {
            var d3D11Mesh = (D3D11Mesh)mesh;

            if (d3D11Mesh.VertexBuffer == null)
                return;

            var deviceContext = D3D11Renderer.GetDeviceContext();

            // update primitive type
            switch (mesh.PrimitiveType)
            {
                case PrimitiveType.Unknown:
                    throw new ReCraftedException("Cannot update the PrimitiveType because it is 'Unknown'.");

                case PrimitiveType.PointList:
                    deviceContext.InputAssembler.PrimitiveTopology = PrimitiveTopology.PointList;
                    break;
                case PrimitiveType.LineList:
                    deviceContext.InputAssembler.PrimitiveTopology = PrimitiveTopology.LineList;
                    break;
                case PrimitiveType.TriangleList:
                    deviceContext.InputAssembler.PrimitiveTopology = PrimitiveTopology.TriangleList;
                    break;
                case PrimitiveType.TriangleStrip:
                    deviceContext.InputAssembler.PrimitiveTopology = PrimitiveTopology.TriangleStrip;
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }

            deviceContext.InputAssembler.SetVertexBuffers(0, d3D11Mesh.VertexBufferBinding);

            if (mesh.HasIndices)
            {
                deviceContext.InputAssembler.SetIndexBuffer(d3D11Mesh.IndexBuffer, Format.R32_UInt, 0);
                deviceContext.DrawIndexed(mesh.Indices.Length, 0, 0);
            }
            else
            {
                deviceContext.Draw(mesh.Vertices.Length, 0);
            }
        }

        /// <summary>
        /// Disposes the shader.
        /// </summary>
        public override void Dispose()
        {
            InputLayout?.Dispose();
            VertexShader?.Dispose();
            ComputeShader?.Dispose();
            PixelShader?.Dispose();
        }

        // private
        private static string ParseShaderSource(string sourceCode, string file)
        {
            var tempSource = sourceCode;
            var sourceFileInfo = new FileInfo(file);

            while (true)
            {
                var includeStart = tempSource.IndexOf("#include", StringComparison.Ordinal);

                if (includeStart != -1)
                {
                    // add includes
                    var start = includeStart + 8;
                    var end = 0;

                    // seek for end
                    var i = start;
                    var open = false; 
                    while (true)
                    {
                        if (tempSource[i] == '"')
                        {
                            if (open)
                            {
                                end = i;
                                break;
                            }

                            start = i+1;
                            open = true;
                        }
                        i++;
                    }

                    // ok
                    var includeFile = sourceFileInfo.DirectoryName + "\\" + tempSource.Substring(start, end-start);

                    tempSource = tempSource.Remove(includeStart, end+1 - includeStart);
                    tempSource = tempSource.Insert(includeStart, File.ReadAllText(includeFile));
                }
                else
                    break;
            }

            return tempSource;
        }
    }
}