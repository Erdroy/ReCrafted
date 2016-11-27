// ReCrafted © 2016 Damian 'Erdroy' Korczowski

using System;
using System.Collections.Generic;
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

        private static PrimitiveType _lastPrimitiveType = PrimitiveType.Unknown;

        private ShaderMeta _meta;

        private VertexShader _vertexShader;
        private PixelShader _pixelShader;
        private ComputeShader _computeShader;

        private InputLayout _inputLayout;

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

            if (hasVs)
            {
                // compile vertex shader
                var bytecode = ShaderBytecode.CompileFromFile(shaderFile, pass.VertexShader, "vs_" + pass.Profile);
                _vertexShader = new VertexShader(device, bytecode);

                // build inputLayout inputElements
                var inputElements = new List<InputElement>();
               
                foreach (var inputElement in pass.VertexLayoutInputs)
                {
                    inputElements.Add(new InputElement(inputElement.InputSignature, 0, D3D11Format.FromString(inputElement.InputType), 0));
                }

                // create inputLayout
                _inputLayout = new InputLayout(device, bytecode, inputElements.ToArray());
            }

            if (hasPs)
            {
                // compile pixel shader
                var bytecode = ShaderBytecode.CompileFromFile(shaderFile, pass.PixelShader, "ps_" + pass.Profile);
                _pixelShader = new PixelShader(device, bytecode);
            }

            if (hasCs)
            {
                // compile compute shader
                var bytecode = ShaderBytecode.CompileFromFile(shaderFile, pass.ComputeShader, "cs_" + pass.Profile);
                _computeShader = new ComputeShader(device, bytecode);
            }

            if (hasConstBuffer)
            {
                var cbuffer = meta.ConstantBuffers[0];

                _backingBuffer = new DataBuffer(cbuffer.Size);
                _dataPointer = _backingBuffer.DataPointer;
                _constantBuffer = Buffer.Create(device, BindFlags.ConstantBuffer, ref _dataPointer, cbuffer.Size, ResourceUsage.Dynamic, CpuAccessFlags.Write);
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
        /// <param name="slot">The field slot.</param>
        /// <param name="value">The value.</param>
        public override void SetValue<T>(int slot, T value)
        {
            throw new NotImplementedException();
        }

        /// <summary>
        /// Set texture at slot.
        /// </summary>
        /// <param name="slot">The slot.</param>
        /// <param name="texture">The texture.</param>
        public override void SetTexture(int slot, Texture2D texture)
        {
            var deviceContext = D3D11Renderer.GetDeviceContext();
            deviceContext.PixelShader.SetShaderResource(slot, ((D3D11Texture2D)texture).ResourceView);
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
        /// Draws mesh using this shader.
        /// </summary>
        /// <param name="mesh">The mesh.</param>
        public override void Draw(Mesh mesh)
        {
            var d3D11Mesh = (D3D11Mesh)mesh;

            if(d3D11Mesh.VertexBuffer == null)
                throw new ReCraftedException("");

            var deviceContext = D3D11Renderer.GetDeviceContext();

            if(_vertexShader != null)
                deviceContext.VertexShader.Set(_vertexShader);

            if (_pixelShader != null)
                deviceContext.PixelShader.Set(_pixelShader);

            if (_computeShader != null)
                deviceContext.ComputeShader.Set(_computeShader);

            if (_inputLayout != null)
                deviceContext.InputAssembler.InputLayout = _inputLayout;

            // update primitive type
            if (_lastPrimitiveType != mesh.PrimitiveType)
            {
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

                _lastPrimitiveType = mesh.PrimitiveType;
            }

            // update constant buffer if needed
            if (_isDirty)
            {
                DataStream stream;
                deviceContext.MapSubresource(_constantBuffer, MapMode.WriteDiscard, MapFlags.None, out stream);

                // update
                stream.Position = 0;
                stream.Write(_backingBuffer.DataPointer, 0, _backingBuffer.Size);

                stream.Dispose();
                deviceContext.UnmapSubresource(_constantBuffer, 0);
            }

            // apply constant buffer
            deviceContext.VertexShader.SetConstantBuffer(0, _constantBuffer);
            deviceContext.PixelShader.SetConstantBuffer(0, _constantBuffer);

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
            _inputLayout?.Dispose();
            _vertexShader?.Dispose();
            _computeShader?.Dispose();
            _pixelShader?.Dispose();
        }
    }
}