// ReCrafted (c) 2016-2018 Always Too Late

using System;
using SharpDX.D3DCompiler;
using SharpDX.Direct3D11;
using SharpDX.DXGI;
using Device = SharpDX.Direct3D11.Device;

namespace ReCrafted.VoxelEditor.Rendering
{
    /// <summary>
    /// Shader class.
    /// </summary>
    public class Shader : IDisposable
    {
        private VertexShader _vertexShader;
        private PixelShader _pixelShader;
        private InputLayout _inputLayout;

        private Device _device;

        /// <summary>
        /// Applies the shader to the current state.
        /// </summary>
        public void Apply()
        {
            var context = _device.ImmediateContext;

            context.InputAssembler.InputLayout = _inputLayout;
            context.VertexShader.Set(_vertexShader);
            context.PixelShader.Set(_pixelShader);
        }

        /// <summary>
        /// Disposes the shader.
        /// </summary>
        public void Dispose()
        {
            _vertexShader.Dispose();
            _pixelShader.Dispose();
            _inputLayout.Dispose();
        }

        /// <summary>
        /// Creates shader from file.
        /// </summary>
        /// <param name="fileName">The shader file name.</param>
        /// <returns>The created shader.</returns>
        public static Shader FromFile(string fileName)
        {
            // WARNING: Lot's of hardcoded stuff, only one shader is now supported, till we will get some meta data from shader.
            // Also we don't have constant buffer support now.

            var device = Renderer.Current.D3D11Device;

            var vertexShaderByteCode = ShaderBytecode.CompileFromFile(fileName, "VSMain", "vs_4_0");
            var pixelShaderByteCode = ShaderBytecode.CompileFromFile(fileName, "PSMain", "ps_4_0");
            var signature = ShaderSignature.GetInputSignature(vertexShaderByteCode);

            return new Shader
            {
                _device = device,
                _vertexShader = new VertexShader(device, vertexShaderByteCode),
                _pixelShader = new PixelShader(device, pixelShaderByteCode),
                _inputLayout = new InputLayout(device, signature, new[]
                {
                    new InputElement("POSITION", 0, Format.R32G32B32_Float, 0, 0, InputClassification.PerVertexData, 0),
                    new InputElement("UV", 0, Format.R32G32_Float, 12, 0, InputClassification.PerVertexData, 0),
                    new InputElement("COLOR", 0, Format.R32G32B32A32_Float, 20, 0, InputClassification.PerVertexData, 0)
                })
            };
        }
    }
}
