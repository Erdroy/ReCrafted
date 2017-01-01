// ReCrafted © 2016-2017 Damian 'Erdroy' Korczowski

using System;
using SharpDX.Direct3D11;

namespace ReCrafted.Graphics.Renderers.D3D11
{
    /// <summary>
    /// D3D11Sampler class.
    /// </summary>
    internal sealed class D3D11Sampler : Sampler
    {
        private static SamplerState _lastSamplerState;

        /// <summary>
        /// Sampler state.
        /// </summary>
        public SamplerState SamplerState;

        /// <summary>
        /// Initialize sampler.
        /// </summary>
        /// <param name="type">The sampler type.</param>
        /// <param name="anisoLevel">The anisotropic level.</param>
        protected override void Init(Type type, int anisoLevel)
        {
            var device = D3D11Renderer.GetDevice();

            SamplerStateDescription desc;
            switch (type)
            {
                case Type.PointClamped:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Clamp,
                        AddressV = TextureAddressMode.Clamp,
                        AddressW = TextureAddressMode.Clamp,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.MinMagMipPoint,
                        MaximumLod = float.MaxValue
                    };
                    break;
                case Type.PointWrap:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Wrap,
                        AddressV = TextureAddressMode.Wrap,
                        AddressW = TextureAddressMode.Wrap,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.MinMagMipPoint,
                        MaximumLod = float.MaxValue
                    };
                    break;
                case Type.PointMirror:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Mirror,
                        AddressV = TextureAddressMode.Mirror,
                        AddressW = TextureAddressMode.Mirror,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.MinMagMipPoint,
                        MaximumLod = float.MaxValue
                    };
                    break;
                case Type.LinearClamped:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Clamp,
                        AddressV = TextureAddressMode.Clamp,
                        AddressW = TextureAddressMode.Clamp,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.MinMagMipLinear,
                        MaximumLod = float.MaxValue
                    };
                    break;
                case Type.LinearWrap:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Wrap,
                        AddressV = TextureAddressMode.Wrap,
                        AddressW = TextureAddressMode.Wrap,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.MinMagMipLinear,
                        MaximumLod = float.MaxValue
                    };
                    break;
                case Type.LinearMirror:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Mirror,
                        AddressV = TextureAddressMode.Mirror,
                        AddressW = TextureAddressMode.Mirror,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.MinMagMipLinear,
                        MaximumLod = float.MaxValue
                    };
                    break;
                case Type.AnisoClamped:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Clamp,
                        AddressV = TextureAddressMode.Clamp,
                        AddressW = TextureAddressMode.Clamp,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.Anisotropic,
                        MaximumAnisotropy = anisoLevel,
                        MaximumLod = float.MaxValue
                    };
                    break;
                case Type.AnisoWrap:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Wrap,
                        AddressV = TextureAddressMode.Wrap,
                        AddressW = TextureAddressMode.Wrap,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.MinMagMipLinear,
                        MaximumAnisotropy = anisoLevel,
                        MaximumLod = float.MaxValue
                    };
                    break;
                case Type.AnisoMirror:
                    desc = new SamplerStateDescription
                    {
                        AddressU = TextureAddressMode.Clamp,
                        AddressV = TextureAddressMode.Clamp,
                        AddressW = TextureAddressMode.Clamp,
                        ComparisonFunction = Comparison.Always,
                        Filter = Filter.MinMagMipLinear,
                        MaximumAnisotropy = anisoLevel,
                        MaximumLod = float.MaxValue
                    };
                    break;
                default:
                    throw new ArgumentOutOfRangeException(nameof(type), type, null);
            }

            SamplerState = new SamplerState(device, desc);
        }

        /// <summary>
        /// Apply the sampler.
        /// </summary>
        /// <param name="slot">The slot.</param>
        public override void Apply(int slot)
        {
            if (_lastSamplerState == SamplerState)
                return;

            _lastSamplerState = SamplerState;

            var deviceContext = D3D11Renderer.GetDeviceContext();
            deviceContext.PixelShader.SetSampler(slot, SamplerState);
        }

        /// <summary>
        /// Dispose the sampler.
        /// </summary>
        public override void Dispose()
        {
            
        }
    }
}